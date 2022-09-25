#pragma once
/* Node Class for the graph
* This is for the Node class of the graph
* The graph will be a 2d array of the grid
* for each spot in the grid.... if its not blue
* make it a node. if its a node then let it know where it is
* there will be a thing that lets you know what if its there or not bitwise
*/
#include <assert.h>
#include <glm/glm.hpp>
#include <vector>

namespace principia {
	namespace lvlgen {
		struct GraphIndex {
			int_fast8_t x = -1;
			int_fast8_t y = -1;

			int_fast8_t& operator[](int i) {
				assert(i > -1 && i < 2);
				return *(&x + i);
			}
			bool operator==(const GraphIndex& gi) const {
				return ((gi.x == x) && (gi.y == y));
			}

			//bool operator==(const GraphIndex& lhs, const GraphIndex& rhs) {
			//	return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
			//}

			inline bool IsValid() {
				// The last bit decides if its a true index or not
				// if 0, then its valid, if 1 then its invalid
				return x >= 0;
			}
			inline void SetIndex(bool b, int_fast8_t x_, int_fast8_t y_) {
				b ? x = x_ : x = -1;
				y = y_;
			}

			GraphIndex() { x = -1; y = -1; }
			GraphIndex(GraphIndex* gi) : x(gi->x), y(gi->y) {};
			GraphIndex(const GraphIndex& gi) : x(gi.x), y(gi.y) {};
			GraphIndex(int_fast8_t _x, int_fast8_t _y) : x(_x), y(_y) {};
			GraphIndex(int _x, int _y) : x(_x), y(_y) {};
		};

		struct GraphInfo {
			int_fast16_t multi_directional = 0;
			int_fast16_t src = 0;
			int_fast16_t dst = 0;

			GraphInfo(const GraphInfo& gi) : multi_directional(gi.multi_directional) {};
			GraphInfo() {};
		};

		struct GraphNode 
		{
			GraphIndex pos;
			GraphInfo info;

			GraphIndex up;
			GraphIndex down;
			GraphIndex left;
			GraphIndex right;

			GraphNode() {};
			GraphNode(const GraphIndex& p, const GraphInfo& i, const GraphIndex& u, const GraphIndex& d, const GraphIndex& l, const GraphIndex& r) :
				pos(p), info(i), up(u), down(d), left(l), right(r) {};
			GraphNode(const GraphIndex&p, const GraphInfo& i) :
				pos(p), info(i) {
				up = GraphIndex(); down = GraphIndex(); left = GraphIndex(); right = GraphIndex();
			}

			inline bool IsValid() { return pos.IsValid(); }
			inline bool CanGoUp() { return up.IsValid(); }
			inline bool CanGoDown() { return down.IsValid(); }
			inline bool CanGoLeft() { return left.IsValid(); }
			inline bool CanGoRight() { return right.IsValid(); }

			void SetMultiDirectional() {
				int vertical = int(up.IsValid() || down.IsValid());
				int horizonatal = int(left.IsValid() || right.IsValid());
				info.multi_directional = int_fast16_t((vertical + horizonatal) == 2);
			}
			void SetSource(bool b) { info.src = (int_fast16_t)b; }
			void SetDest(bool b) { info.dst = (int_fast16_t)b; }
		};

		struct Graph {
			std::vector<std::vector<GraphNode>> nodes;
			int_fast8_t width;
			int_fast8_t height;
			Graph(int w, int h) : width(w), height(h) {
				nodes = std::vector<std::vector<GraphNode>>(w, std::vector<GraphNode>(h));
			} 
			
			void build(std::vector<std::vector<bool>> grid) {
				//Builds the center, not the edges
				for (int_fast8_t r = 1; r < width - 1; ++r) {
					for (int_fast8_t c = 1; c < height - 1; ++c) {
						buildNode(r, c, grid);
					}
				}
				//Builds the first and the last row
				for (int_fast8_t c = 0;  c < height; ++c) {
					buildCornerNode(0, c, grid);
					buildCornerNode(width - 1, c, grid);
				}
				//Builds the first and the last column
				for (int_fast8_t r = 1; r < width - 1; ++r) {
					buildCornerNode(r, 0, grid);
					buildCornerNode(r, height - 1, grid);
				}
			}

			void buildNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid) {
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
			
			void buildCornerNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid) {
				nodes[r][c].pos.SetIndex(grid[r][c], r, c);
				if (c - 1 >= 0) nodes[r][c].up.SetIndex(grid[r][c - 1], r, c - 1);
				if (c + 1 <= height - 1) nodes[r][c].down.SetIndex(grid[r][c + 1], r, c + 1);
				if (r - 1 >= 0)nodes[r][c].left.SetIndex(grid[r - 1][c], r - 1, c);
				if (r + 1 <= width - 1) nodes[r][c].right.SetIndex(grid[r + 1][c], r + 1, c);

				nodes[r][c].SetMultiDirectional();
			}
			
		private:
			void dfs_helper(std::vector<GraphNode> graph, const GraphNode* src, const GraphNode* dst) {
				if (src->pos == dst->pos)
					return;
			}
		};
	}
}
