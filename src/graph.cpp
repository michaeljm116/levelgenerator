#include "graph.h" 
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace principia {
	namespace lvlgen{

		Graph::Graph(int w, int h) : width(w), height(h) {
			nodes = std::vector<std::vector<GraphNode>>(w, std::vector<GraphNode>(h));
		}

		void Graph::build(std::vector<std::vector<bool>> grid)
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

		//This most of the nodes except for the end pieces, it skips many of the checks you need
		void Graph::buildNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid) {
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

		// Same as above but with the checks since its in a corner
		void Graph::buildCornerNode(int_fast8_t r, int_fast8_t c, std::vector<std::vector<bool>>& grid) {
			nodes[r][c].pos.SetIndex(grid[r][c], r, c);
			if (c - 1 >= 0) nodes[r][c].up.SetIndex(grid[r][c - 1], r, c - 1);
			if (c + 1 <= height - 1) nodes[r][c].down.SetIndex(grid[r][c + 1], r, c + 1);
			if (r - 1 >= 0)nodes[r][c].left.SetIndex(grid[r - 1][c], r - 1, c);
			if (r + 1 <= width - 1) nodes[r][c].right.SetIndex(grid[r + 1][c], r + 1, c);

			nodes[r][c].SetMultiDirectional();
		}

		// This is the main graph traversal algorithm:
		// its a Depth First Search approach that gives
		// Preference to nodes that are closer to the target
		std::vector<GraphNode> Graph::FindTarget()
		{
			//This is a node of the source
			auto curr = getNode(src);

			//This is the main path
			auto path = std::vector<GraphNode>(); 

			//This is a history of everything visited
			auto visited = std::vector<std::unordered_set<int_fast16_t>>();
			auto firstVisit = std::unordered_set<int_fast16_t>();
			firstVisit.insert(curr.pos.ToInt16());
			visited.push_back(firstVisit);

			//This is a history of all changes in direction
			auto multi_directionals = std::vector < std::pair<GraphNode, std::vector<GraphNode::Direction>>>(); 
			auto directions = DetermineDirection(curr, GraphIndex(-1,-1));
			multi_directionals.push_back(std::make_pair(curr, directions));

			//Main Loop, while the target hasnt been found, keep traversing the graph
			while (curr.pos != dst) {
				//This is the current path you're traversing
				//If it ends in a dead end/loop, it wont be preserved, else you'll push it to the visited list
				auto temp_path = std::vector<GraphNode>(1,curr);

				//This is a list of everything you've visited, 
				//Same as above for preservation, keep if good, delete if bad
				auto temp_visited = visited.back();

				//This goes through the graph in the current direction, and returns a flag if necessary
				GraphFlags flags = TraverseGraph(temp_visited, temp_path, curr, directions.back());

				switch (flags)
				{
				case principia::lvlgen::Graph::FLAG_NONE: {
					std::cout << "\nWTF JUST HAPPENED BRO???\n";
					break; 
				}
				case principia::lvlgen::Graph::FLAG_TARGET_FOUND: {
					path.insert(path.end(), temp_path.begin(), temp_path.end());
					for (auto n : path) {
						n.info.path = 1;
						nodes[n.pos.x][n.pos.y].info.path = 1;
					}
					return path;
					break; 
				}
				case principia::lvlgen::Graph::FLAG_MULTI_DIRECTIONAL: {
					const auto& prev_node = temp_path[temp_path.size() - 2].pos;
					directions = DetermineDirection(curr, prev_node);
					multi_directionals.push_back(std::make_pair(curr, directions));
					path.insert(path.end(), temp_path.begin(), temp_path.end());
					visited.push_back(temp_visited);
					break; 
				}
				case principia::lvlgen::Graph::FLAG_ALREADY_VISITED: {
					//While you're on something you've visited
					while (temp_visited.find(curr.pos.ToInt16()) != temp_visited.end()) {
						//Change direction
						directions.pop_back();
						if (!directions.empty()) { //If there is another direction to go to, go there
							curr = multi_directionals.back().first;
							temp_visited = visited.front();
						}
						else { //There's no other direction to go to, so you have to go back to a previous multi directional Node
							multi_directionals.pop_back();
							directions = multi_directionals.back().second;
							//This should then loop back and directions will pop_back and continue...
						}
						if (multi_directionals.empty()) {
							// This should never happen, unless the graph is untraversable, in which case stop everything
							std::cout << "CANNOT REACH THE TARGET";
							return std::vector<GraphNode>();
						}
					}
					break; 
				}
				case principia::lvlgen::Graph::FLAG_DEAD_END: {
					directions.pop_back();
					if (directions.empty()) {
						multi_directionals.pop_back();
						curr = multi_directionals.back().first;
						directions = multi_directionals.back().second;
					}
					break; 
				}
				default:
					break;
				}
				if (multi_directionals.size() > 1000) {
					std::cout << "ERROR: STUCK IN SOME INF LOOP IDK HOW THO";
					return std::vector<GraphNode>();
				}
			}

			return std::vector<GraphNode>();
		}

		// This sets the source and destination and then finds the target
		std::vector<GraphNode> Graph::FindTarget(GraphIndex src, GraphIndex dst)
		{
			this->src = src; this->dst = dst;
			return FindTarget();
		}

		// Traverse Node takes a node and a direction and moves it according to the direction
		void Graph::TraverseNode(GraphNode& node, GraphNode::Direction dir)
		{
			auto& next = node[(int)dir];
			if (next.IsValid()) {
				node = nodes[next.x][next.y];
			}
		}

		Graph::GraphFlags Graph::TraverseGraph(std::unordered_set<int_fast16_t>& temp_visited, std::vector<GraphNode>& temp_path, GraphNode& curr, const GraphNode::Direction& direction)
		{
			Graph::GraphFlags flag = FLAG_NONE;
			while (flag == FLAG_NONE)
			{
				TraverseNode(curr, direction);
				flag = CheckForFlags(temp_visited, curr);
				temp_visited.insert(curr.pos.ToInt16());
				temp_path.push_back(curr);
			}
			return flag;
		}

		// Determine Direction tries to send a list of next possible directions by:
		// Seeing which potential direction is closer to the target
		// It returns a sorted vector (LIFO) based on closest to farthest
		std::vector<GraphNode::Direction> Graph::DetermineDirection(const GraphNode& src, const GraphIndex& prev_node) const {
			// find the differences
			std::vector<std::pair<glm::f32, int>> diffs;
			for (int i = 1; i < 5; ++i) {
				if (src[i].IsValid() && (src[i] != prev_node)) {
					glm::f32 diff = glm::sqrt(glm::pow((dst.x - src[i].x),2) + glm::pow((src[i].y - dst.y),2));
					diffs.push_back(std::make_pair(diff, i));
				}
			}
			//sort them by distance in a stack-like-order
			std::sort(diffs.begin(), diffs.end(), [](const std::pair<glm::f32, int>& a, const std::pair<glm::f32, int>& b) {
				return a.first > b.first;
				});

			//make a vector and return;
			std::vector<GraphNode::Direction> dirs;
			dirs.reserve(diffs.size());
			for (auto d : diffs)
				dirs.emplace_back((GraphNode::Direction)d.second);
			return dirs;
		}

		// Check for flags takes a node and finds if any flags have been flagged
		Graph::GraphFlags Graph::CheckForFlags(const std::unordered_set<int_fast16_t>& visited, const GraphNode& curr) const
		{
			auto flag = FLAG_NONE;
			if (visited.find(curr.pos.ToInt16()) != visited.end())
				return FLAG_ALREADY_VISITED;
			if (curr.pos == dst)
				return FLAG_TARGET_FOUND;
			if (curr.info.multi_directional)
				flag = FLAG_MULTI_DIRECTIONAL;
			if (!curr.IsValid())
				return FLAG_DEAD_END;
			return flag;
		}

		//TODO: PROBLEM YOU CAN ONLY RETURN ONE THING
	}
}