#define _USE_MATH_DEFINES
#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    GrayscaleImage copiedImage(image); //copy
    int height = image.get_height();
    int width = image.get_width();
    int** copiedData = copiedImage.get_data();
    int** data = image.get_data();
    int radius = kernelSize / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;
            int count = 0;

            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {
                    int neighborY = y + ky;
                    int neighborX = x + kx;
                    if (neighborY < 0 || neighborY >= height || neighborX < 0 || neighborX >= width) {
                        sum += 0;  //padding
                    } else {
                        int pixelValue = copiedData[neighborY][neighborX];
                        sum += pixelValue;
                    }
                    count++;
                }
            }
            int meanValue = sum / count;
            data[y][x] = std::min(255, std::max(0, meanValue));
        }
    }
}


// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // gaussian kernel
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0;
    int radius = kernelSize / 2;

    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            double exponent = -(x * x + y * y) / (2 * sigma * sigma);
            kernel[y + radius][x + radius] = (1 / (2 * M_PI * sigma * sigma)) * exp(exponent);
            sum += kernel[y + radius][x + radius]; //normalization
        }
    }

    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            kernel[y][x] /= sum;
        }
    }

    int height = image.get_height();
    int width = image.get_width();
    int** data = image.get_data();
    int** newData = new int*[height];

    for (int i = 0; i < height; ++i) {
        newData[i] = new int[width];
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double weightedSum = 0.0;
            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {
                    int neighborY = y + ky;
                    int neighborX = x + kx;
                    if (neighborY < 0 || neighborY >= height || neighborX < 0 || neighborX >= width) {
                        weightedSum += 0;
                    } else {
                        weightedSum += data[neighborY][neighborX] * kernel[ky + radius][kx + radius];
                    }
                }
            }

            newData[y][x] = static_cast<int>(floor(weightedSum));
            newData[y][x] = std::min(255, std::max(0, newData[y][x]));
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = newData[y][x];
        }
        delete[] newData[y];
    }
    delete[] newData;
}


void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    double sigma = 1.0;
    GrayscaleImage blurredImage(image);
    apply_gaussian_smoothing(blurredImage, kernelSize, sigma);

    int height = image.get_height();
    int width = image.get_width();
    int** originalData = image.get_data();
    int** blurredData = blurredImage.get_data();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double edgeImage = static_cast<double>(originalData[y][x]) - static_cast<double>(blurredData[y][x]);
            double sharpenedValue = static_cast<double>(originalData[y][x]) + (amount * edgeImage);
            if (sharpenedValue < 0) {
                originalData[y][x] = 0;
            } else if (sharpenedValue > 255) {
                originalData[y][x] = 255;
            } else {
                originalData[y][x] = static_cast<int>(sharpenedValue);
            }
        }
    }
}
