#include "Crypto.h"

#include <deque>

#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    //TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    int width = image.get_width();
    int height = image.get_height();
    int imageSize = width * height;

    // 3. Determine the total bits required based on message length.
    int required = message_length * 7;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (required > imageSize) {
        throw std::runtime_error("Image does not have enough pixels.");
    }

    int count = 0;
    std::vector<int> temp_array;

    // 5. Calculate the starting pixel from the message_length knowing that
    // the last LSB to extract is in the last pixel of the image.
    for (int row = height - 1; row >= 0 && count < required; row--) {
        for (int col = width - 1; col >= 0 && count < required; col--) {
            int pixel = image.get_pixel(row, col);
            int lsb = pixel & 1;
            temp_array.push_back(lsb);
            count++;

            // 6. Extract LSBs from the image pixels and return the result.
            if (temp_array.size() == 7) {
                std::reverse(temp_array.begin(), temp_array.end());
                LSB_array.insert(LSB_array.begin(), temp_array.begin(), temp_array.end());
                temp_array.clear();
            }
        }
    }

    if (!temp_array.empty()) {
        std::reverse(temp_array.begin(), temp_array.end());
        LSB_array.insert(LSB_array.end(), temp_array.begin(), temp_array.end());
    }

    return LSB_array;
}

// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.


    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("Invalid LSB array size.");
    }

    int lsb_index = 0;

    while (lsb_index < LSB_array.size()) {
        std::deque<int> deque;

        for (int j = 0; j < 7; j++) {
            deque.push_back(LSB_array[lsb_index + j]);
        }

        int ascii_value = 0;
        for (int i = 0; i < 7; i++) {
            ascii_value += deque[i] * (1 << (6 - i));
        }

        char character = static_cast<char>(ascii_value);

        message.push_back(character);

        lsb_index += 7;
    }

    return message;

}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.


    for (char c : message) {
        std::bitset<7> bits(static_cast<int>(c));

        std::deque<int> temp_bits;
        for (int i = 0; i < 7; i++) {
            temp_bits.push_front(bits[i]);
        }

        for (int bit : temp_bits) {
            LSB_array.push_back(bit);
        }

        temp_bits.clear();
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    //TODO: Your code goes here.

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    int width = image.get_width();
    int height = image.get_height();
    int imageSize = width * height;
    int LSBsize = LSB_array.size();

    if (LSBsize > imageSize) {
        throw std::runtime_error("Error: Image does not have enough pixels to store the message.");
    }

    // 2. Find the starting pixel based on the message length knowing that
    // the last LSB to embed should end up in the last pixel of the image.
    int start = imageSize - LSBsize;
    int startY = start % width;
    int startX = start / width;

    int LsbIndex = 0;

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (int i = startX; i < height && LsbIndex < LSBsize; i++) {
        for (int j = (i == startX ? startY : 0); j < width && LsbIndex < LSBsize; j++) {
            int pixel = image.get_pixel(i, j);

            int new_LSB = LSB_array[LsbIndex++];
            pixel = (pixel & 0xFE) | new_LSB;

            image.set_pixel(i, j, pixel);
        }
    }

    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    // with the embedded message.
    SecretImage secret_image(image);
    return secret_image;
}
