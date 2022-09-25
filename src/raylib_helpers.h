#pragma once
#include <raylib.hpp>
#include "texture.h"
#include <glm/glm.hpp>
#include "graph.h"

typedef unsigned char l_byte;
using namespace principia;

std::string display_vec3(glm::vec3 v);
Color lvlgen_pixel_to_color(lvlgen::Pixel p);
void copy_lvlgen_to_raylib(Image* rl, const lvlgen::Image& lg);

int DisplayMenuOptions(int& active);
void DisplayKey(bool active);

void LoadImage();

void copy_graph_to_raylib(Image* rl, const lvlgen::Graph& g);
Color graph_node_to_color(lvlgen::GraphNode n);