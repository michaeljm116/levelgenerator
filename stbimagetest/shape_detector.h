#pragma once
#include "image_convert.h"
namespace principia {
	namespace lvlgen {
		enum ImageShape {
			kShapeUnknown,
			kShapeCircle,
			kShapeSquare,
			kShapeRectangle,
			kShapeL,
			kShapePlus
		};

		struct ShapeData {
			ImageShape shape;
			vec2 size;
			vec2 position;
		};

		struct Simpixel {
			ImageColor color = ImageColor::kColorBlack;
			int row = 0; int col = 0;
			Simpixel(const ImageColor& _color, int _row, int _col) : color(_color), row(_row), col(_col) {}
			Simpixel() {};
		};

		struct Shape {
			ImageColor color;
			std::vector<Simpixel> pixels;
			bool fill = false;
			int GetWidth() { return max_x - min_y; }
			int GetHeight() { return max_y - min_y; }
			vec2 GetPos() { return vec2(); }
			void UpdateSize(int row, int col) {
				min_x = std::min(min_x, row);
				max_x = std::max(max_x, row);
				min_y = std::min(min_y, col);
				max_y = std::max(max_y, col);
			}
		private:
			int min_x = INT32_MAX;
			int max_x = INT32_MIN;
			int min_y = INT32_MAX;
			int max_y = INT32_MIN;
		};


		typedef std::vector<std::vector<bool>> GraphMemo;
		class ShapeDetector
		{
		public:
			ShapeDetector(const ColorGraph& graph);
			~ShapeDetector();
			void DetectShapes(const ColorGraph& graph);
			std::vector<ShapeData>& GetShapeData() { return _data; }


		private:
			std::vector<ShapeData> _data;
			ColorGraph _shapes_graph;
			int rows = 0, columns = 0;

			void DetectCircle(const ColorGraph& graph);
			void DetectSquare(const ColorGraph& graph);
			void DetectRectangle(const ColorGraph& graph);

			bool dfs(int row, int col, ImageColor color, Shape& shape, ColorGraph& graph, GraphMemo& visited);
		};

		

	}
}
