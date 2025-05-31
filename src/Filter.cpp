#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.

    int rows = image.get_height();
    int cols = image.get_width();

    int** copiedArr = new int*[rows];
    for (int i = 0; i < rows; i++) {
        copiedArr[i] = new int[cols];
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            copiedArr[i][j] = image.get_pixel(i, j);
        }
    }

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.


    for (int i = 0; i < rows; i++) {
        for (int j = 0 ; j < cols; j++) {

            double tempSum = 0.0;
            int total = kernelSize * kernelSize;
            int slide = (kernelSize / 2);

            for (int k = i - slide; k <= i + slide; k++) {
                for (int m = j - slide; m <= j + slide; m++) {
                    if (k >= 0 && k < rows && m >= 0 && m < cols) {
                        tempSum += copiedArr[k][m];
                    }
                }
            }
            // 3. Update each pixel with the computed mean.

            image.set_pixel(i, j, static_cast<int>(tempSum / static_cast<double>(total)));
        }
    }



    for (int i = 0; i < rows; i++) {
        delete[] copiedArr[i];
    }
    delete[] copiedArr;

}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
  	//TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    int height = image.get_height();
    int width = image.get_width();
    double **kernelArr = new double *[kernelSize];
    for (int i = 0; i < kernelSize; i++) {
        kernelArr[i] = new double[kernelSize];
    }

    int slide = kernelSize / 2;

    double sigma2 = 2 * sigma * sigma;
    double tempSum1 = 0.0;

    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            int row = i - slide;
            int col = j - slide;

            kernelArr[i][j] = exp(-(row * row + col * col) / sigma2) / (M_PI * sigma2);
            tempSum1 += kernelArr[i][j];
        }
    }

    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernelArr[i][j] = kernelArr[i][j] / tempSum1;
        }
    }

    int **copiedData = new int *[height];
    for (int i = 0; i < height; i++) {
        copiedData[i] = new int[width];
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copiedData[i][j] = image.get_pixel(i,j);
        }
    }

    // 3. For each pixel, compute the weighted sum using the kernel.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double tempSum = 0.0;
            for (int k = 0; k < kernelSize; k++) {
                for (int m = 0; m < kernelSize; m++) {
                    int row = i + (k - slide);
                    int col = j + (m - slide);
                    if (row >= 0 && row < height && col >= 0 && col < width) {
                        tempSum += copiedData[row][col] * kernelArr[k][m];
                    }
                }
            }
            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(i,j, static_cast<int>(tempSum));
        }
    }

    for (int i = 0; i < height; i++) {
        delete[] copiedData[i];
    }
    delete[] copiedData;

    for (int i = 0; i < kernelSize; i++) {
        delete[] kernelArr[i];
    }
    delete[] kernelArr;
}




// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage &image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.

    GrayscaleImage blurred(image.get_width(), image.get_height());

    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            blurred.set_pixel(i, j, image.get_pixel(i, j));
        }
    }

    apply_gaussian_smoothing(blurred, kernelSize, 1.0);

    int rows = image.get_height();
    int cols = image.get_width();

    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double org_pix = image.get_pixel(i, j);
            double blurred_val = blurred.get_pixel(i, j);

            double high_freq_component = (org_pix - blurred_val);
            double temp = org_pix + (amount * high_freq_component);

			// 3. Clip values to ensure they are within a valid range [0-255].

            if (temp < 0) {
                temp = 0;
            }
            else if (temp > 255) {
                temp = 255;
            }

            image.set_pixel(i, j, temp);
        }
    }
}
