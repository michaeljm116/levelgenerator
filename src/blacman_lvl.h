#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "component_types.h"
//#include "../lib/xxhash/xxhash.hpp"

namespace principia {
	namespace lvlgen {

		struct PrefabData {
			std::string dir;
			std::string name;
			bool can_serialize = false;
			bool load_needed = true;
			bool save = false;
		};

		struct ObjectData
		{
			ObjectData() {};
			ObjectData(std::string mat) {
				material = mat[0];
				for (size_t i = 1; i < mat.size(); ++i) {
					material *= mat[i] + mat[i - 1];
				} 
				//material = xxh::xxhash<32, char>(mat);
			}
			std::string name;
			int64_t flags = COMPONENT_NODE;
			ObjectType type;
			CollisionType col_type;
			glm::vec3 size;
			glm::vec3 pos;
			glm::vec3 rot = glm::vec3(0);
			int material;
			bool dynamic = true;
			PrefabData prefab;
		};

		struct BlacmanLvl {
			BlacmanLvl() {};
			BlacmanLvl(std::vector<ObjectData> w, std::vector<ObjectData> f, std::vector<ObjectData> c, ObjectData floor) :
				walls(w), food(f), characters(c), floor(floor) {};
			std::vector<ObjectData> walls;
			std::vector<ObjectData> food;
			std::vector<ObjectData> characters;
			ObjectData floor;
		};
	}
}