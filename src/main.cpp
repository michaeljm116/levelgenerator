#include "image_convert.h"
#include "pacman_lvl_detector.h"
#include "serialize.h"

using namespace principia;
int main() {
	auto img = lvlgen::Image("assets/pacmap001.bmp");
	auto converter = lvlgen::ImageConverter(img);
	auto lvl = lvlgen::BlacmanLvlConverter(converter.ColorMatrix()).GetLvl();
	auto serializer = lvlgen::Serializer();
	serializer.SaveLvl("testlvl4.xml", lvl, 1);
	return 0;
}

