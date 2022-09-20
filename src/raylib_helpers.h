#pragma once
#include <raylib.hpp>
#include <raygui.h>
#include "texture.h"
#include <glm/glm.hpp>

typedef unsigned char l_byte;
using namespace principia;

std::string display_vec3(glm::vec3 v);
Color lvlgen_pixel_to_color(lvlgen::Pixel p);
void copy_lvlgen_to_raylib(Image* rl, const lvlgen::Image& lg);