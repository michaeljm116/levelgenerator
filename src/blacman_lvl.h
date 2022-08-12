#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "component_types.h"
namespace principia {
	namespace lvlgen {
		struct ObjectData
		{
			ObjectData() {};
			std::string name;
			int64_t flags = COMPONENT_NODE;
			ObjectType type;
			CollisionType col_type;
			glm::vec3 size;
			glm::vec3 pos;
		};

		struct BlacmanLvl {
			BlacmanLvl() {};
			BlacmanLvl(std::vector<ObjectData> w, std::vector<ObjectData> f, std::vector<ObjectData> c) :
				walls(w), food(f), characters(c) {};
			std::vector<ObjectData> walls;
			std::vector<ObjectData> food;
			std::vector<ObjectData> characters;
		};
	}
}