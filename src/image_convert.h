#ifndef  IMAGE_CONVERT_H
#define IMAGE_CONVERT_H
#include "image_data.h"
#include "texture.h"

namespace principia {
	namespace lvlgen {

		enum ImageColor {
			kColorRed,
			kColorPurple,
			kColorBlue,
			kColorGreen,
			kColorYellow,
			kColorOrange,
			kColorWhite,
			kColorBlack,
			kColorGrey,
			kColorCyan
		};

		typedef std::vector<std::vector<ImageColor>>  ColorGraph;

		class ImageConverter
		{
		public:
			~ImageConverter();
			ImageConverter(const Image& image);
			inline ColorGraph& ColorMatrix() { return _color_matrix; }
			inline const vec3& ColorConstants(int&& i) { return color_constants[i]; }
		private:
			ColorGraph _color_matrix;
			void ConvertImageToMatrix(const Image& image);

			ImageColor DetectNearestColor(const lvlgen::Pixel& pixel);

			const std::array<vec3, 10> color_constants = {
			vec3(255, 0, 0), vec3(255, 0, 255), vec3(0, 0, 255),
			vec3(0, 255, 0), vec3(255, 255, 0), vec3(255, 125, 0),
			vec3(255, 255, 255), vec3(0, 0, 0), vec3(125, 125, 125), vec3(0, 200, 200)};

		};
	}
}

#endif // ! IMAGE_CONVERT_H