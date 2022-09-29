#include "graph.h" 
#include <unordered_set>
#include <algorithm>

using namespace principia;

lvlgen::Graph::Graph(int w, int h) : width(w), height(h) {
	nodes = std::vector<std::vector<GraphNode>>(w, std::vector<GraphNode>(h));
}


void lvlgen::Graph::build(std::vector<std::vector<bool>> grid)
{
	//Builds the center, not the edges
	for (int_fast8_t r = 1; r < width - 1; ++r) {
		for (int_fast8_t c = 1; c < height - 1; ++c) {
			buildNode(r, c, grid);
		}
	}
	//Builds the first and the last row
	for (int_fast8_t c = 0; c < height; ++c) {
		buildCornerNode(0, c, grid);
		buildCornerNode(width - 1, c, grid);
	}
	//Builds the first and the last column
	for (int_fast8_t r = 1; r < width - 1; ++r) {
		buildCornerNode(r, 0, grid);
		buildCornerNode(r, height - 1, grid);
	}
}

void lvlgen::Graph::buildNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid) {
	assert(r - 1 >= 0);
	assert(c - 1 >= 0);
	assert(r + 1 <= width);
	assert(c + 1 <= height);

	nodes[r][c].pos.SetIndex(grid[r][c], r, c);
	nodes[r][c].up.SetIndex(grid[r][c - 1], r, c - 1);
	nodes[r][c].down.SetIndex(grid[r][c + 1], r, c + 1);
	nodes[r][c].left.SetIndex(grid[r - 1][c], r - 1, c);
	nodes[r][c].right.SetIndex(grid[r + 1][c], r + 1, c);

	nodes[r][c].SetMultiDirectional();
}

void lvlgen::Graph::buildCornerNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid) {
	nodes[r][c].pos.SetIndex(grid[r][c], r, c);
	if (c - 1 >= 0) nodes[r][c].up.SetIndex(grid[r][c - 1], r, c - 1);
	if (c + 1 <= height - 1) nodes[r][c].down.SetIndex(grid[r][c + 1], r, c + 1);
	if (r - 1 >= 0)nodes[r][c].left.SetIndex(grid[r - 1][c], r - 1, c);
	if (r + 1 <= width - 1) nodes[r][c].right.SetIndex(grid[r + 1][c], r + 1, c);

	nodes[r][c].SetMultiDirectional();
}

std::vector<lvlgen::GraphNode> principia::lvlgen::Graph::FindTarget()
{
	return std::vector<lvlgen::GraphNode>();
}

std::vector<lvlgen::GraphNode> principia::lvlgen::Graph::FindTarget(lvlgen::GraphIndex src, lvlgen::GraphIndex dst)
{
	this->src = src; this->dst = dst;
	return FindTarget();
}

std::vector<lvlgen::GraphIndex> lvlgen::Graph::DetermineDirection(const lvlgen::GraphNode& src, const lvlgen::GraphNode& dst, const GraphNode& prev) const {
	// find the differences
	std::vector<std::pair<glm::f32, int>> diffs;
	for (int i = 1; i < 5; ++i) {
		if (src[i].IsValid() && (src[i] != prev.pos)) {
			glm::f32 diff = glm::sqrt(glm::exp2(src[i].x - dst.pos.x) + glm::exp2(src[i].y - dst.pos.y));
			diffs.push_back(std::make_pair(diff, i));
		}
	}

	//sort them by distance
	std::sort(diffs.begin(), diffs.end(), [](const std::pair<glm::f32, int>& a, const std::pair<glm::f32, int>& b) {
		return a.first < b.first;
		});

	//make a vector and return;
	std::vector<lvlgen::GraphIndex> valids = std::vector<lvlgen::GraphIndex>(diffs.size());
	for (int i = 0; i < diffs.size(); ++i)valids[i] = src[diffs[i].second];
	return valids;
};