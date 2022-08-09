#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <assert.h>

namespace principia {
	namespace lvlgen {
		struct Pixel {
			unsigned char r = 0, g = 0, b = 0, a = 1;
			unsigned char& operator[](int i) {
				assert(i > -1 && i < 4);
				return *(&r + i);
			}
			unsigned char& const operator[](const int i) const {
				assert(i > -1 && i < 4);
				return const_cast<unsigned char&>(*(&r + i));
			}
			
			Pixel() {};
			Pixel(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 1) : r(_r), g(_g), b(_b), a(_a) {};

		};

		struct Image {
			int width, height, channels;
			std::vector<std::vector<Pixel>> data;
			Image(int image_width, int image_height, int image_channels) : width(image_width), height(image_height), channels(image_channels) {
				data = std::vector(width, std::vector<Pixel>(height));
			}
			Image(std::string txtr_file) {
				LoadImageFromTexture(txtr_file);
			}
			void LoadImageFromTexture(std::string txtr_file) {
				stbi_uc* pixels = stbi_load(txtr_file.c_str(), &width, &height, &channels, 0);
				data = std::vector(width, std::vector<Pixel>(height));
				unsigned int i, j, k;
				for (k = 0; k < channels; ++k) {
					for (j = 0; j < height; ++j) {
						for (i = 0; i < width; ++i) {
							unsigned int index = k + channels * i + channels * width * j;
							data[j][i][k] = pixels[index];
						}
					}
				}
				stbi_image_free(pixels);
			}
		};
	}
}