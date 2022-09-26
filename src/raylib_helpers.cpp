#include "raylib_helpers.h"
#include <raygui.h>

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

int DisplayMenuOptions(int& active)
{
	return GuiToggleGroup(Rectangle({ 165, 400, 140, 25 }), "Load Level\nSave Level\nDisplay Graph", active);
}

void DisplayKey(bool active) {
	if(active)
		GuiPanel(Rectangle({ 560, 25 + 180, 100, 160 }),"White = Node\nBlack = Wall\nPurple = Multi Directional\nBlue = SRC\nRed = DST\nGreen = SEARCH");
}

void LoadImage()
{
	auto img = lvlgen::Image("assets/pacmap001.bmp");
	
}

void copy_graph_to_raylib(Image* rl, lvlgen::Graph& g)
{
	assert(rl->width == g.width);
	assert(rl->height == g.height);
	for (size_t i = 0; i < g.width; ++i) {
		for (size_t j = 0; j < g.height; ++j) {
			//DrawCircle(i, j, lvlgen_pixel_to_color(lg.data[i][j]));
			ImageDrawPixel(rl, j, i, graph_node_to_color(g.nodes[i][j]));

		}
	}
}

Color graph_node_to_color(lvlgen::GraphNode& n)
{
	if (n.IsValid()) {
		if (n.info.path) 
			return BLUE;
		else if (n.info.dst) 
			return RED;
		else if (n.info.src) 
			return GREEN;
		else if (n.info.multi_directional) 
			return PURPLE;
		else return LIGHTGRAY;
	}
	else
		return BLACK;
	return Color();
}

glm::ivec2 Mouse_To_Img_Pos(float scale, Vector2 mouse_position)
{
	auto div = 1 / scale;
	return glm::ivec2(mouse_position.y * div, mouse_position.x * div);
}

void ResetGraphSrc(lvlgen::Graph& g)
{
	for (auto& v : g.nodes) {
		for (auto& n : v) {
			n.SetSource(false);
		}
	}
}

void ResetGraphDst(lvlgen::Graph& g)
{
	for (auto& v : g.nodes) { for (auto& n : v) n.SetDest(false); }
}

void SetGraphSrc(lvlgen::Graph& g, glm::ivec2 src)
{
	ResetGraphSrc(g);
	g.nodes[src.x][src.y].SetSource(true);
	g.src = g.nodes[src.x][src.y].pos;
}

void SetGraphDst(lvlgen::Graph& g, glm::ivec2 dst)
{
	ResetGraphDst(g);
	g.nodes[dst.x][dst.y].SetDest(true);
	g.dst = g.nodes[dst.x][dst.y].pos;
}

