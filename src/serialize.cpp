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
			SaveCamAndLights(p_root, &doc);
			p_root->InsertEndChild(SaveNode(lvl.floor, &doc));
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

			rotation->SetAttribute("x", obj.rot.x);
			rotation->SetAttribute("y", obj.rot.y);
			rotation->SetAttribute("z", obj.rot.z);

			scale->SetAttribute("x", obj.size.x);
			scale->SetAttribute("y", obj.size.y);
			scale->SetAttribute("z", obj.size.z);

			transform->InsertFirstChild(position);
			transform->InsertAfterChild(position, rotation);
			transform->InsertEndChild(scale);

			node->InsertEndChild(transform);

			//---------- Material ---------- 
			XMLElement* material = doc->NewElement("Material");
			material->SetAttribute("ID", obj.material);
			node->InsertEndChild(material);

			//---------- ObjectID ----------
			if (obj.type != ObjectType::NONE) {
				XMLElement* object = doc->NewElement("Object");
				object->SetAttribute("ID", (int)obj.type);
				node->InsertEndChild(object);
			}

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
		XMLElement* Serializer::SaveCamAndLights(XMLNode* root, XMLDocument* doc)
		{
			//Camera
			XMLElement* cam_node = doc->NewElement("Node");
			cam_node->SetAttribute("Name", "Camera");
			cam_node->SetAttribute("hasChildren", false);
			cam_node->SetAttribute("eFlags", 131091);
			cam_node->SetAttribute("gFlags", 0);
			cam_node->SetAttribute("Dynamic", true);


			//---------- Transform ---------- 
			XMLElement* transform = doc->NewElement("Transform");
			XMLElement* position = doc->NewElement("Position");
			XMLElement* rotation = doc->NewElement("Rotation");
			XMLElement* scale = doc->NewElement("Scale");

			position->SetAttribute("x", 15);
			position->SetAttribute("y", 45);
			position->SetAttribute("z", 12.410);

			rotation->SetAttribute("x", 0);
			rotation->SetAttribute("y", 0);
			rotation->SetAttribute("z", 0.0001);

			scale->SetAttribute("x", 1);
			scale->SetAttribute("y", 1);
			scale->SetAttribute("z", 1);

			transform->InsertFirstChild(position);
			transform->InsertAfterChild(position, rotation);
			transform->InsertEndChild(scale);
						
			XMLElement* aspect = doc->NewElement("AspectRatio");
			aspect->SetAttribute("ratio", 1.7777778);
			XMLElement* fov = doc->NewElement("FOV");
			fov->SetAttribute("fov", 104);

			cam_node->InsertEndChild(transform);
			cam_node->InsertEndChild(aspect);
			cam_node->InsertEndChild(fov);
			root->InsertEndChild(cam_node);
		

			//Lights
			XMLElement* light_node = doc->NewElement("Node");
			light_node->SetAttribute("Name", "Light");
			light_node->SetAttribute("hasChildren", false);
			light_node->SetAttribute("eFlags", 131083);
			light_node->SetAttribute("gFlags", 0);
			light_node->SetAttribute("Dynamic", true);

			//---------- Light Transform ---------- 
			XMLElement* ltransform = doc->NewElement("Transform");
			XMLElement* lposition = doc->NewElement("Position");
			XMLElement* lrotation = doc->NewElement("Rotation");
			XMLElement* lscale = doc->NewElement("Scale");

			lposition->SetAttribute("x", 14.3);
			lposition->SetAttribute("y", 27.141);
			lposition->SetAttribute("z", 13.323);

			lrotation->SetAttribute("x", 0);
			lrotation->SetAttribute("y", 0);
			lrotation->SetAttribute("z", 0);

			lscale->SetAttribute("x", 1);
			lscale->SetAttribute("y", 1);
			lscale->SetAttribute("z", 1);

			ltransform->InsertFirstChild(lposition);
			ltransform->InsertAfterChild(lposition, lrotation);
			ltransform->InsertEndChild(lscale);


			XMLElement* color = doc->NewElement("Color");
			color->SetAttribute("r", 255);
			color->SetAttribute("g", 255);
			color->SetAttribute("b", 255);

			XMLElement* intensity = doc->NewElement("Intensity");
			intensity->SetAttribute("i", 1000);

			XMLElement* ID = doc->NewElement("ID");
			ID->SetAttribute("id", 0);

			light_node->InsertEndChild(ltransform);
			light_node->InsertEndChild(color);
			light_node->InsertEndChild(intensity);
			light_node->InsertEndChild(ID);
			root->InsertEndChild(light_node);

			return nullptr;
		}
	}
}