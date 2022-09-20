#include "image_convert.h"
#include "pacman_lvl_detector.h"
#include "serialize.h"

#include "raylib_helpers.h"
#include "graph.h"

using namespace principia;

Image* lvlgen_to_raylib_image(const lvlgen::Image& img) {
	Image* ret = new Image();
	ret->width = img.width;
	ret->height = img.height;
	ret->mipmaps = 1;
	ret->format = PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

	auto* data = new std::vector<std::vector<std::vector<l_byte>>>(4, std::vector<std::vector<l_byte>>(img.data.size(), std::vector<l_byte>(img.data[0].size())));
	for (size_t k = 0; k < 4; ++k) {
		for (size_t i = 0; i < img.data.size(); ++i) {
			for (size_t j = 0; j < img.data[0].size(); ++j) {
				data->at(k).at(j)[i] = img.data[i][j][k];
			}
		}
	}
	ret->data = data;
	return ret;
}

void draw_lvlgen_img(lvlgen::Image img) {
	for (size_t i = 0; i < img.width; ++i) {
		for (size_t j = 0; j < img.height; ++j) {
			Color col = lvlgen_pixel_to_color(img.data[i][j]);
			DrawPixel(i, j, col);
			img.data[i][j].DisplayPixel();
		}
		std::cout << "\n\n";
	}
}

auto raylib_to_lvlgen_image(const Image& img) {
	std::vector<std::vector<lvlgen::Pixel>> data = std::vector<std::vector<lvlgen::Pixel>>(img.width, std::vector<lvlgen::Pixel>(img.height));
	for (int k = 0; k < 4; ++k) {
		for (int j = 0; j < img.height; ++j) {
			for (int i = 0; i < img.width; ++i) {
				char* d = (char*)img.data;
				data[j][i][k] = d[k + 4 * i + 4 * img.width * j];
			}
		}
	}
	lvlgen::Image ret = lvlgen::Image(img.width, img.height, 4);
	ret.data = data;
	return ret;
}

auto print_raylib_img_data(const Image& img) {
	//ret.data = &img.data;
	std::string channels[5] = { "Red", "Blue", "Green", "Alpha" "\n" };
	std::cout << "\nImage Start:\n" << channels[0] << std::endl;
	for (size_t k = 0; k < 4; ++k) {
		for (size_t i = 0; i < img.width; ++i) {
			for (size_t j = 0; j < img.height; ++j) {
				auto* helpa = ((std::vector<std::vector<std::vector<l_byte>>>*)img.data);
				auto hd = helpa->at(k).at(j)[i];
				std::cout << " " << (int)hd << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "\nend\n" << channels[k + 1] << std::endl;
	}
}

std::vector<std::vector<bool>> colormatrix_to_wallsmatrix(lvlgen::ColorGraph cg) {
	std::vector<std::vector<bool>> ret = std::vector<std::vector<bool>>(cg.size(), std::vector<bool>(cg[0].size()));
	for (int r = 0; r < cg.size(); ++r) {
		for (int c = 0; c < cg[0].size(); ++c) {
			if (cg[r][c] == lvlgen::kColorBlue) ret[r][c] = false;
			else ret[r][c] = true;
		}
	}
	return ret;
}

int main(int argc, char** argv) {
	auto img = lvlgen::Image("assets/pacmap001.bmp");
	/*auto converter = lvlgen::ImageConverter(img);
	auto lvl = lvlgen::BlacmanLvlConverter(converter.ColorMatrix()).GetLvl();

	auto grid = colormatrix_to_wallsmatrix(converter.ColorMatrix());
	lvlgen::Graph graf = lvlgen::Graph(grid.size(), grid[0].size());
	graf.build(grid);

	auto firstwall_pos = lvl.walls[0].pos;
	auto firstwall_size = lvl.walls[0].size;
	auto target_pos = glm::vec3(16.f, 1.f, 31.5f);
	auto target_size = glm::vec3(15.f, 1.f, 0.5f);

	std::cout << "Position (" << display_vec3(firstwall_pos) << ")\n";
	std::cout << "Size (" << display_vec3(firstwall_size) << ")\n";
	std::cout << "\nTarget Pos (" << display_vec3(target_pos) << ")\n";
	std::cout << "Target Size (" << display_vec3(target_size) << ")\n\n";*/
	
	//assert(firstwall_pos == target_pos);
	//assert(firstwall_size == target_size);
	

	//auto serializer = lvlgen::Serializer();
	//serializer.SaveLvl("C:\\dev\\PrincipiumGames\\ShinyAfroMan\\Assets\\Levels\\Test\\Scenes\\testlvl5.xml", lvl, 1);
	//return 0;*/

	int scale = 20;
	int width = img.width * scale;
	int height = img.height * scale;

	InitWindow(width + width / 3, height, "raylib [core] example - basic window"); 

	auto rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");

	//auto* ltr	= lvlgen_to_raylib_image(img); 
	//auto rtl	= raylib_to_lvlgen_image(rimg);
	//auto both	= lvlgen_to_raylib_image(rtl); 
	//auto back	= raylib_to_lvlgen_image(both);
		

	//ImageClearBackground(&rimg, BLACK);
	/*
	
	To Justin: Sorry for the suuuuper late reply bro! I've had to do lots of asking around. I
	
	
	*/

	
	copy_lvlgen_to_raylib(&rimg, img);
	ImageResizeNN(&rimg, width, height);

	auto txtr = LoadTextureFromImage(rimg);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexture(txtr, 0, 0, WHITE);
		EndDrawing();
	}

	CloseWindow();
	

	/*
	// Initialization
	//---------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 600;

	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(screenWidth, screenHeight, "raygui - portable window");

	// General variables
	Vector2 mousePosition = { 0 };
	Vector2 windowPosition = { 500, 200 };
	Vector2 panOffset = mousePosition;
	bool dragWindow = false;

	SetWindowPosition(windowPosition.x, windowPosition.y);

	bool exitWindow = false;

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!exitWindow && !WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		mousePosition = GetMousePosition();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			if (CheckCollisionPointRec(mousePosition, Rectangle({ 0, 0, screenWidth, 20 })))
			{
				dragWindow = true;
				panOffset = mousePosition;
			}
		}

		if (dragWindow)
		{
			windowPosition.x += (mousePosition.x - panOffset.x);
			windowPosition.y += (mousePosition.y - panOffset.y);

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragWindow = false;

			SetWindowPosition(windowPosition.x, windowPosition.y);
		}
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		exitWindow = GuiWindowBox(Rectangle({ 0, 0, screenWidth, screenHeight }), "#198# PORTABLE WINDOW");

		DrawText(TextFormat("Mouse Position: [ %.0f, %.0f ]", mousePosition.x, mousePosition.y), 10, 40, 10, DARKGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;*/
}