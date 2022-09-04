#pragma once
//#include "../lib/stb/stb_image.h"
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
			unsigned const char& operator[](const int i) const {
				assert(i > -1 && i < 4);
				return const_cast<unsigned char&>(*(&r + i));
			}
			void operator=(const Pixel& p) {
				r = p.r; g = p.g; b = p.b; a = p.a;
			}
			
			Pixel() {};
			Pixel(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 1) : r(_r), g(_g), b(_b), a(_a) {};

		};

		struct Image {
			int width, height, channels;
			std::vector<std::vector<Pixel>> data = std::vector<std::vector<Pixel>>();
			Image(int image_width, int image_height, int image_channels) : width(image_width), height(image_height), channels(image_channels) {
				data = std::vector(width, std::vector<Pixel>(height));
			}
			Image(std::string txtr_file) {
				LoadImageFromTexture(txtr_file);
			}
			void LoadImageFromTexture(std::string txtr_file);
			void DrawImageFromData(std::string txtr_file);
		};
	}
}