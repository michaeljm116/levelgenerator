#pragma once

#include "shape_detector.h"

namespace principia {
	namespace lvlgen {
		ShapeDetector::ShapeDetector(const ColorGraph& graph)
		{
			rows = graph.size();
			columns = graph[0].size();
		}

		ShapeDetector::~ShapeDetector()
		{
		}
		void ShapeDetector::DetectShapes(const ColorGraph& graph)
		{
			GraphMemo _visited_memo = std::vector<std::vector<bool>>(graph.size(), std::vector<bool>(graph[0].size(), false));
			for (int r = 0; r < rows; ++r) {
				for (int c = 0; c < columns; ++c) {
					if (!_visited_memo[r][c]) {
						//dfs(r, c, kColorBlue, )
						//dfs(r, c, );
					}
				}
			}
		}
		void ShapeDetector::DetectCircle(const ColorGraph& graph)
		{
		}
		void ShapeDetector::DetectSquare(const ColorGraph& graph)
		{
		}
		void ShapeDetector::DetectRectangle(const ColorGraph& graph)
		{
		}
		bool ShapeDetector::dfs(int row, int col, ImageColor color, Shape& shape, ColorGraph& graph, GraphMemo& visited)
		{
			int invalid = (row >= 0) | (row < rows) | (col >= 0) | (col < columns);
			if (invalid > 0) return true;
			if (visited[row][col]) return true;
			else {
				visited[row][col] = true;
				if (graph[row][col] == color) {
					shape.pixels.push_back(Simpixel(color, row, col));
					shape.UpdateSize(row, col);
					dfs(row - 1, col - 1, color, shape, graph, visited);
					dfs(row - 1, col + 1, color, shape, graph, visited);
					dfs(row + 1, col - 1, color, shape, graph, visited);
					dfs(row + 1, col + 1, color, shape, graph, visited);
					return true;
				}
				else { 	return true; }
			}

		}
	}
}