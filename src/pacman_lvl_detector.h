/* pacman_lvl_detector.h
*  this takes a graph of different colors
*  and assigns them things based off what 
*  they are
*  yeet
* Mike Murrell Copywright(c) 2022
*/
#ifndef PACMAN_LVL_DETECTOR_H
#define PACMAN_LVL_DETECTOR_H


#include "image_convert.h"
#include "blacman_lvl.h"
namespace principia {
	namespace lvlgen {

		typedef std::vector<std::vector<bool>> Memo;


		class BlacmanLvlConverter
		{
		public:
			BlacmanLvlConverter(const ColorGraph& graph);
			~BlacmanLvlConverter();
			void GraphToLevel(const ColorGraph& graph);
			void DetectObject(int r, int c, const ColorGraph& graph);

			BlacmanLvl GetLvl();
			inline const std::vector<ImageData>& GetWalls() { return _walls; }
			inline const std::vector<ImageData>& GetFood() { return _food; }
			inline const std::vector<ImageData>& GetCharacters() { return _characters; }

			void AddWall(const ImageData& d);
			inline void AddFood(const ImageData& d) { _food.push_back(d); }
			inline void AddCharacter(const ImageData& d) { _characters.push_back(d); }
		
		private:
			//This section is for finding and storing the data of the level
			std::vector<ImageData> _walls;
			std::vector<ImageData> _food;
			std::vector<ImageData> _characters;
			glm::vec2 level_size;

			//This section is for the things related to wall detection
			Memo _visited;
			vec2 DetectWallSize(int r, int c, const ColorGraph& graph);
			int CheckWallDown(int r, int c, const ColorGraph& graph);
			int CheckWallRight(int r, int c, const ColorGraph& graph);

			void ConvertObjectPosition(ObjectData& obj);
		};

	}

}


#endif // !PACMAN_LVL_DETECTOR_H