#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    height = image.get_height();
    width = image.get_width();

    if (height != width) {
        throw std::invalid_argument("Image must be square.");
    }

    int n = width;
    int numElements = (n * (n + 1)) / 2;
    upper_triangular = new int[numElements];
    lower_triangular = new int[numElements];
    int** data = image.get_data();
    int upperIndex = 0;
    int lowerIndex = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i <= j)  {
                upper_triangular[upperIndex++] = data[i][j];
            }
            if (i > j) {
                lower_triangular[lowerIndex++] = data[i][j];
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    width = w;
    height = h;

    upper_triangular = upper;
    lower_triangular = lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
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

    GrayscaleImage reconstructedImage(width, height);

    int** imageData = reconstructedImage.get_data();
    int upperIndex = 0;
    int lowerIndex = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                imageData[i][j] = upper_triangular[upperIndex++];
            }
            else {
                imageData[i][j] = lower_triangular[lowerIndex++];
            }
        }
    }

    return reconstructedImage;
}


// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    if (image.get_width() != width || image.get_height() != height) {
        throw std::invalid_argument("Image dimensions do not match with SecretImage dimensions.");
    }

    int** imageData = image.get_data();
    int upperIndex = 0;
    int lowerIndex = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                upper_triangular[upperIndex++] = imageData[i][j];
            }
            if (i >= j) {
                lower_triangular[lowerIndex++] = imageData[i][j];
            }
        }
    }
}


void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    int image_width = get_width();
    int triangularSize = image_width * (image_width + 1) / 2;

    /*// Debug: Print the size of the triangular arrays
    std::cout << "Saving file: " << filename << std::endl;
    std::cout << "Triangular array size: " << triangularSize << std::endl;*/
    file << width << " " << height << "\n";

    for (int i = 0; i < triangularSize; ++i) {
        file << upper_triangular[i];
        if (i < triangularSize - 1) {
            file << " ";
        }
    }
    file << "\n";

    // Write lower triangular array
    for (int i = 0; i < triangularSize; i++) {
        file << lower_triangular[i];
        if (i < triangularSize - 1) {
            file << " ";
        }
    }
    file << "\n";
    file.close();
}

SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    int width, height;
    file >> width >> height;
    int upperTriangularSize = width * (width + 1) / 2;
    int lowerTriangularSize = width * (width - 1) / 2;
    int* upper_triangular = new int[upperTriangularSize];
    int* lower_triangular = new int[lowerTriangularSize];

    for (int i = 0; i < upperTriangularSize; ++i) {
        file >> upper_triangular[i];
    }

    for (int i = 0; i < lowerTriangularSize; ++i) {
        file >> lower_triangular[i];
    }
    file.close();
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
