#include "serialize.h"

namespace principia {
	namespace lvlgen {
		Serializer::Serializer()
		{

		}
		Serializer::~Serializer()
		{
		}
		void Serializer::SaveLvl(std::string file, BlacmanLvl lvl, int lvl_num)
		{
			XMLDocument doc;
			XMLError e_result;
			XMLNode* p_root = doc.NewElement("Root");
			doc.InsertFirstChild(p_root);
			XMLElement* scene_number = doc.NewElement("Scene");
			scene_number->SetAttribute("Num", lvl_num);

			p_root->InsertFirstChild(scene_number);

			for (auto o : lvl.characters) {
				p_root->InsertEndChild(SaveNode(o, &doc));
			}
			for(auto o : lvl.walls){
				p_root->InsertEndChild(SaveNode(o, &doc));
			}
			for (auto o : lvl.food) {
				p_root->InsertEndChild(SaveNode(o, &doc));
			}
			e_result = doc.SaveFile(file.c_str());
			if (e_result != 0) std::cout << "FAILED TO SAVE";
		}
		XMLElement* Serializer::SaveNode(const ObjectData& obj, XMLDocument * doc)
		{
			XMLElement* node = doc->NewElement("Node");
			node->SetAttribute("Name", obj.name.c_str());
			node->SetAttribute("hasChildren", false);
			node->SetAttribute("eFlags", obj.flags);
			node->SetAttribute("gFlags", 0);
			node->SetAttribute("Dynamic", false);

			//---------- Transform ---------- 
			XMLElement* transform = doc->NewElement("Transform");
			XMLElement* position = doc->NewElement("Position");
			XMLElement* rotation = doc->NewElement("Rotation");
			XMLElement* scale = doc->NewElement("Scale");

			position->SetAttribute("x", obj.pos.x);
			position->SetAttribute("y", obj.pos.y);
			position->SetAttribute("z", obj.pos.z);

			rotation->SetAttribute("x", 0);
			rotation->SetAttribute("y", 0);
			rotation->SetAttribute("z", 0);

			scale->SetAttribute("x", obj.size.x);
			scale->SetAttribute("y", obj.size.y);
			scale->SetAttribute("z", obj.size.z);

			transform->InsertFirstChild(position);
			transform->InsertAfterChild(position, rotation);
			transform->InsertEndChild(scale);

			node->InsertEndChild(transform);

			//---------- Material ---------- 
			XMLElement* material = doc->NewElement("Material");
			material->SetAttribute("ID", 0);
			node->InsertEndChild(material);

			//---------- ObjectID ----------
			XMLElement* object = doc->NewElement("Object");
			object->SetAttribute("ID", (int)obj.type);
			node->InsertEndChild(object);

			//---------- Collider ----------
			XMLElement* collision = doc->NewElement("Collider");
			XMLElement* local = doc->NewElement("Local");
			XMLElement* extents = doc->NewElement("Extents");

			collision->SetAttribute("Type", (int)obj.col_type);
			
			extents->SetAttribute("x", obj.size.x);
			extents->SetAttribute("y", obj.size.y);
			extents->SetAttribute("z", obj.size.z);

			local->SetAttribute("x", obj.pos.x);
			local->SetAttribute("y", obj.pos.y);
			local->SetAttribute("z", obj.pos.z);

			collision->InsertFirstChild(local);
			collision->InsertEndChild(extents);
			node->InsertEndChild(collision);

			return node;
		}
	}
}