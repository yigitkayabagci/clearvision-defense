#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    this -> width = image.get_width();
    this -> height = image.get_height();

    int upper_size = (width * width + width) / 2;
    int lower_size = (width * width - width) / 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    // 2. Fill both matrices with the pixels from the GrayscaleImage.


    int upp_idx = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= j) {
                upper_triangular[upp_idx] = image.get_pixel(i, j);
                upp_idx += 1;
            }
        }
    }

    int low_idx= 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i > j) {
                lower_triangular[low_idx] = image.get_pixel(i, j);
                low_idx += 1;
            }
        }
    }

}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;

    int upper_size = (width * width + width) / 2;
    int lower_size = (width * width - width) / 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    for (int i = 0; i < upper_size; i++) {
        upper_triangular[i] = upper[i];
    }

    for (int i = 0; i < lower_size; i++) {
        lower_triangular[i] = lower[i];
    }

    delete[] upper;
    delete[] lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.

    if (upper_triangular != nullptr) {
        delete[] upper_triangular;
        upper_triangular = nullptr;
    }
    if (lower_triangular != nullptr) {
        delete[] lower_triangular;
        lower_triangular = nullptr;
    }
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int upp_index = 0;
    int low_index = 0;

    int upper_size = (width * width + width) / 2;
    int lower_size = (width * width - width) / 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j >= i && upp_index < upper_size) {
                image.set_pixel(i, j, upper_triangular[upp_index]);
                upp_index++;
            }
            if (i > j && low_index < lower_size) {
                image.set_pixel(i, j, lower_triangular[low_index]);
                low_index++;
            }
        }
    }
    return image;
}
// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
  	// TODO: Your code goes here.
    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.
    int rows = image.get_height();
    int cols = image.get_width();

    int upper_size = (cols * cols + cols) / 2;
    int lower_size = (cols * cols - cols) / 2;

    int upp_idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = i; j < cols; j++) {
            if (upp_idx >= upper_size) {
                break;
            }
            upper_triangular[upp_idx] = image.get_pixel(i, j);
            upp_idx += 1;
        }
        if (upp_idx >= upper_size) {
            break;
        }
    }

    int low_idx = 0;
    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < i; j++) {
            if (low_idx >= lower_size) {
                break;
            }
            lower_triangular[low_idx] = image.get_pixel(i, j);
            low_idx += 1;
        }
        if (low_idx >= lower_size) {
            break;
        }
    }
}


// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // TODO: Your code goes here.


    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        throw std::runtime_error("Error: Could not open file for writing: " + filename);
    }

    int width = get_width();
    int height = get_height();

    int upper_size = (width * width + width) / 2;
    int lower_size = (width * width - width) / 2;

    // 1. Write width and height on the first line, separated by a single space.

    outfile << width << " " << height << std::endl;

    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    for (int i = 0; i < upper_size; i++) {
        outfile << upper_triangular[i];
        if (i < upper_size - 1) {
            outfile << " ";
        }
    }
    outfile << std::endl;


    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.

    for (int i = 0; i < lower_size; i++) {
        outfile << lower_triangular[i];
        if (i < lower_size - 1) {
            outfile << " ";
        }
    }
    outfile << std::endl;
    outfile.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.

    std::ifstream infile(filename);

    if (!infile.is_open()) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    std::string temp;
    std::getline(infile, temp);

    size_t ind = temp.find(' ');
    if (ind == std::string::npos) {
        throw std::runtime_error("Error: Incorrect file format in " + filename);
    }


    // 2. Calculate the sizes of the upper and lower triangular arrays.

    int width = std::stoi(temp.substr(0, ind));
    int height = std::stoi(temp.substr(ind + 1));

    int upper_size = (width * width + width) / 2;
    int lower_size = (width * width - width) / 2;

    // 3. Allocate memory for both arrays.

    int* upper_triangular = new int[upper_size];
    int* lower_triangular = new int[lower_size];

    // 4. Read the upper_triangular array from the second line, space-separated.

    std::string uppertriangle_Line;
    std::getline(infile, uppertriangle_Line);
    std::istringstream iss(uppertriangle_Line);

    for (int i = 0; i < upper_size; i++) {
        if (!(iss >> upper_triangular[i])) {
            delete[] upper_triangular;
            delete[] lower_triangular;
            throw std::runtime_error("Error: Failed to read upper triangular data from file " + filename);
        }
    }

    // 5. Read the lower_triangular array from the third line, space-separated.

    std::string lowertriangle_Line;
    std::getline(infile, lowertriangle_Line);
    std::istringstream iss2(lowertriangle_Line);

    for (int j = 0; j < lower_size; j++) {
        if (!(iss2 >> lower_triangular[j])) {
            delete[] upper_triangular;
            delete[] lower_triangular;
            throw std::runtime_error("Error: Failed to read lower triangular data from file " + filename);
        }
    }
	// 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    infile.close();
    return SecretImage(width, height, upper_triangular, lower_triangular);
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
