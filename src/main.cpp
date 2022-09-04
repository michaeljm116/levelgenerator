#include "image_convert.h"
#include "pacman_lvl_detector.h"
#include "serialize.h"

#include <raylib.h>

using namespace principia;

std::string display_vec3(glm::vec3 v) {
	return "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
}

Image lvlgen_to_raylib_image(const lvlgen::Image& img) {
	Image ret;
	ret.width = img.width;
	ret.height = img.height;
	ret.mipmaps = 1;
	ret.format = PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	
	//ret.data = new char[img.width * img.height * sizeof(lvlgen::Pixel)];
	//memcpy(ret.data, &img.data, img.width * img.height * sizeof(lvlgen::Pixel));

	//static_cast<void*>(img.data);

	auto* data = new std::vector<std::vector<std::vector<char>>>(4, std::vector<std::vector<char>>(img.data.size(), std::vector<char>(img.data[0].size())));
		//<std::vector<std::array<char, 4>>>(img.data.size(), std::vector<std::array<char,4>>(img.data[0].size()));
	for (size_t k = 0; k < 4; ++k) {
		for (size_t i = 0; i < img.data.size(); ++i) {
			for (size_t j = 0; j < img.data[0].size(); ++j) {
				data->at(k).at(i)[j] = img.data[i][j][k];
			}
		}
	}
	ret.data = data;
	//ret.data = std::copy(std::data(img.data);
	return ret;
}

auto raylib_to_lvlgen_image(const Image& img) {
	std::vector<std::vector<lvlgen::Pixel>> ret = std::vector<std::vector<lvlgen::Pixel>>(img.width, std::vector<lvlgen::Pixel>(img.height));

	for (int k = 0; k < 4; ++k) {
		for (int j = 0; j < img.height; ++j) {
			for (int i = 0; i < img.width; ++i) {
				char* d = (char*)img.data;
				
				ret[j][i][k] = d[k + 4 * i + 4 * img.width * j];
			}
		}
	}

	return ret;
}

int main(int argc, char** argv) {
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
	//return 0;*/

	int scale = 20;
	int width = img.width * scale;
	int height = img.height * scale;

	InitWindow(width, height, "raylib [core] example - basic window");
	auto rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
	auto hi = raylib_to_lvlgen_image(rimg);
	auto test_img = Image(lvlgen_to_raylib_image(img));
	//ImageResizeNN(test_img, width, height);
	auto copy = ImageCopy(test_img);
	ImageResizeNN(&copy,width, height);
	//ImageDrawCircle(&rimg, width / 2, height / 2, 5, BEIGE);
	//auto txtr = LoadTextureFromImage(rimg);
	auto txtr = LoadTextureFromImage(copy);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexture(txtr, 0, 0, WHITE);
		EndDrawing();
	}

	CloseWindow();
}