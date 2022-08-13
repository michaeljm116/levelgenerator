#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "pacman_lvl_detector.h"    
#include "../lib/tinyxml2/tinyxml2.h"
using namespace tinyxml2;
namespace principia {
	namespace lvlgen {
		class Serializer {
		public:
			Serializer();
			~Serializer();
			void SaveLvl(std::string file, BlacmanLvl lvl, int lvl_num);
			XMLElement* SaveNode(const ObjectData& obj, XMLDocument* doc);
			XMLElement* SaveCamAndLights(XMLNode* root, XMLDocument* doc);
		private:

		};
	}
}

#endif // !SERIALIZE_H