#include "raylib_helpers.h"

std::string display_vec3(glm::vec3 v) {
	return "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
}
Color lvlgen_pixel_to_color(lvlgen::Pixel p) {
	Color c; c.r = (int)p.r; c.g = (int)p.g; c.b = (int)p.b; c.a = (int)p.a;
	//p.DisplayPixel();
   //return { p.r, p.g, p.b, p.a };
	return c;
}
void copy_lvlgen_to_raylib(Image* rl, const lvlgen::Image& lg) {
	assert(rl->width == lg.width);
	assert(rl->height == lg.height);
	for (size_t i = 0; i < lg.width; ++i) {
		for (size_t j = 0; j < lg.height; ++j) {
			//DrawCircle(i, j, lvlgen_pixel_to_color(lg.data[i][j]));
			ImageDrawPixel(rl, j, i, lvlgen_pixel_to_color(lg.data[i][j]));

		}
	}

}