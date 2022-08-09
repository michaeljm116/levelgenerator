#pragma once
#include "image_convert.h"

namespace principia {
	namespace lvlgen {	

		ImageConverter::~ImageConverter()
		{
		}

		ImageConverter::ImageConverter(const Image& image)
		{
			_color_matrix = std::vector<std::vector<ImageColor>>(image.width, std::vector<ImageColor>(image.height));
			ConvertImageToMatrix(image);
		}

		void ImageConverter::ConvertImageToMatrix(const Image& image)
		{
			for (auto i = 0; i < image.width; ++i) {
				for (auto j = 0; j < image.height; ++j) {
					_color_matrix[i][j] = DetectNearestColor(image.data[i][j]);
				}
			}
		}

		ImageColor ImageConverter::DetectNearestColor(const lvlgen::Pixel& pixel)
		{
			ImageColor ret = lvlgen::kColorBlack;
			float min = FLT_MAX;
			for (int c = 0; c < 9; c++) {
				float total = 0;
				for (int i = 0; i < 3; ++i) {
					auto cc = color_constants[c];
					float diff = cc[i] - float(pixel[i]);
					total += diff * diff;
				}
				total = sqrt(total);
				if (total < min) {
					ret = ImageColor(c);
					min = total;
				}
			}
			return ret;
		}
	}
}