#include "Crypto.h"
#include "GrayscaleImage.h"
#include <vector>
#include <stdexcept>
#include <bitset>

// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length


std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    GrayscaleImage image = secret_image.reconstruct();
    int width = image.get_width();
    int height = image.get_height();
    int total_pixels = width * height;
    int total_bits_required = message_length * 7;

    if (total_bits_required > total_pixels) {
        throw std::runtime_error("Error: Image does not have enough pixels to hold the message.");
    }

    int start_pixel_index = total_pixels - total_bits_required;
    for (int i = start_pixel_index; i < total_pixels; ++i) {
        int x = i % width;
        int y = i / width;

        int pixel_value = image.get_pixel(y, x);
        int lsb = pixel_value & 1;
        LSB_array.push_back(lsb);
    }
    return LSB_array;
}



// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("Error: LSB array size is not a multiple of 7.");
    }

    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        int ascii_value = 0;

        for (int bit_index = 0; bit_index < 7; ++bit_index) {
            ascii_value = (ascii_value << 1) | LSB_array[i + bit_index];
        }

        message += static_cast<char>(ascii_value);
    }

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    for (char c : message) {
        std::bitset<7> binary(c);  // 7 bits for ASCII
        for (int i = 6; i >= 0; --i) {
            LSB_array.push_back(binary[i]);
        }
    }
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    int width = image.get_width();
    int height = image.get_height();
    int total_pixels = width * height;

    if (LSB_array.size() > total_pixels) {
        throw std::runtime_error("Not enough pixels in the image to store the entire message.");
    }

    int start_pixel_index = total_pixels - LSB_array.size();
    int** pixel_data = image.get_data();
    int bit_index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixel_index = y * width + x;
            if (pixel_index < start_pixel_index) {
                continue;
            }
            int pixel_value = pixel_data[y][x];

            int new_LSB = LSB_array[bit_index++];
            pixel_value = (pixel_value & ~1) | new_LSB;
            pixel_data[y][x] = pixel_value;
            if (bit_index >= LSB_array.size()) {
                break;
            }
        }
        if (bit_index >= LSB_array.size()) {
            break;
        }
    }
    return SecretImage(image);
}
