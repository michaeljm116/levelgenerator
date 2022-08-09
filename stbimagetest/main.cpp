
#include "image_convert.h"
using namespace principia;
int main() {
	auto img = lvlgen::Image("images/pacmap001.png");
	auto converter = lvlgen::ImageConverter(img);
	return 0;
}

