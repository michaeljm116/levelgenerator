#include "image_convert.h"
#include "pacman_lvl_detector.h"
#include "serialize.h"

using namespace principia;

std::string display_vec3(glm::vec3 v) {
	return "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
}

int main() {
	auto img = lvlgen::Image("assets/pacmap001.bmp");
	auto converter = lvlgen::ImageConverter(img);
	auto lvl = lvlgen::BlacmanLvlConverter(converter.ColorMatrix()).GetLvl();
	
	auto firstwall_pos = lvl.walls[0].pos;
	auto firstwall_size = lvl.walls[0].size;
	auto target_pos = glm::vec3(16.f, 1.f, 31.5f);
	auto target_size = glm::vec3(15.f, 1.f, 0.5f);

	std::cout << "Position (" << display_vec3(firstwall_pos) << ")\n";
	std::cout << "Size (" << display_vec3(firstwall_size) << ")\n";
	std::cout << "\nTarget Pos (" << display_vec3(target_pos) << ")\n";
	std::cout << "Target Size (" << display_vec3(target_size) << ")\n\n";
	
	//assert(firstwall_pos == target_pos);
	//assert(firstwall_size == target_size);

	auto serializer = lvlgen::Serializer();
	serializer.SaveLvl("C:\\dev\\PrincipiumGames\\ShinyAfroMan\\Assets\\Levels\\Test\\Scenes\\testlvl5.xml", lvl, 1);
	return 0;
}