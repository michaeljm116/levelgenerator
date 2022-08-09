#pragma once
#include "image_convert.h"
namespace principia {
	namespace lvlgen {

		class BlacmanLvlConverter
		{
		public:
			BlacmanLvlConverter(const ColorGraph& graph);
			~BlacmanLvlConverter();


		private:
			void DetectWall  (const ColorGraph& graph);
			void DetectFood  (const ColorGraph& graph);
			void DetectPlayer(const ColorGraph& graph);
			void DetectEnemey(const ColorGraph& graph);
			std::vector<ImageData> _data;
		};


	}

}