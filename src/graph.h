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
#include <unordered_set>

namespace principia {
	namespace lvlgen {
		
		//16bits
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
			bool operator!=(const GraphIndex& gi) const {
				return ((gi.x != x) || (gi.y != y));
			}

			//bool operator==(const GraphIndex& lhs, const GraphIndex& rhs) {
			//	return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
			//}

			inline bool IsValid() const {
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

		//48bits
		struct GraphInfo {
			int_fast16_t multi_directional = 0;
			int_fast16_t path = 0;
			int_fast8_t src = 0;
			int_fast8_t dst = 0;

			GraphInfo(const GraphInfo& gi) : multi_directional(gi.multi_directional) {};
			GraphInfo() {};
		};

		//128bits
		struct GraphNode 
		{
			//64bits
			GraphInfo info;
			GraphIndex pos;

			//64Bits
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

			// Node[0] = pos, node[1] = up... etc... node[4] = right;
			//GraphIndex& operator[](int i) {
			//	assert(i > -1 && i < 5);
			//	return *(&pos + i);
			//}
			const GraphIndex& operator[](int i) const {
				assert(i > -1 && i < 5);
				return *(&pos + i);
			}

			inline bool IsValid() const { return pos.IsValid(); }
			inline bool CanGoUp() const { return up.IsValid(); }
			inline bool CanGoDown() const { return down.IsValid(); }
			inline bool CanGoLeft() const { return left.IsValid(); }
			inline bool CanGoRight() const { return right.IsValid(); }

			inline void SetMultiDirectional() {
				int vertical = int(up.IsValid() || down.IsValid());
				int horizonatal = int(left.IsValid() || right.IsValid());
				info.multi_directional = int_fast16_t((vertical + horizonatal) == 2);
			}
			inline void SetSource(bool b) { info.src = (int_fast8_t)b; }
			inline void SetDest(bool b) { info.dst = (int_fast8_t)b; }
			inline void SetPath(bool b) { info.path = (int_fast16_t)b; }
		};

		

		struct Graph {
			enum GraphFlags {
				FLAG_TARGET_FOUND,
				FLAG_MULTI_DIRECTIONAL,
				FLAG_ALREADY_VISITED,
				FLAG_DEAD_END
			};

			std::vector<std::vector<GraphNode>> nodes;
			int_fast8_t width;
			int_fast8_t height;

			GraphIndex src;
			GraphIndex dst;
			Graph(int w, int h);

			void build(std::vector<std::vector<bool>> grid);
			void buildNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid);			
			void buildCornerNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid);

			std::vector<GraphNode> FindTarget();
			std::vector<GraphNode> FindTarget(GraphIndex src, GraphIndex dst);
			std::vector<GraphIndex> DetermineDirection(const GraphNode& src, const GraphNode& dst, const GraphNode& prev) const;
			int CheckForFlags(std::unordered_set<GraphIndex>& visited, GraphNode curr);
		};
	}
}
