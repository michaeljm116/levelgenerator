#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace principia {
	namespace lvlgen {
		void Image::LoadImageFromTexture(std::string txtr_file){
			stbi_uc* pixels = stbi_load(txtr_file.c_str(), &width, &height, &channels, 0);
			data = std::vector<std::vector<Pixel>>(width, std::vector<Pixel>(height));
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
	}
}