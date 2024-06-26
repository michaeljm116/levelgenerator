#include "serialize.h"
#include <filesystem>

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != tinyxml2::XML_SUCCESS) { printf("Error: %i\n", a_eResult); return nullptr; }
#endif

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
		XMLElement* Serializer::SaveNode(const ObjectData& obj, XMLDocument* doc)
		{
			XMLElement* node = doc->NewElement("Node");
			node->SetAttribute("Name", obj.name.c_str());
			node->SetAttribute("hasChildren", false);
			node->SetAttribute("eFlags", obj.flags);
			node->SetAttribute("gFlags", 0);
			node->SetAttribute("Dynamic", obj.dynamic);

			if (obj.flags & COMPONENT_PREFAB) {
				node = LoadPrefab(obj);
				return node;
				/*
				XMLElement* prefab_element = doc->NewElement("Prefab");
				XMLElement* prefab_file = doc->NewElement("FilePath");
				XMLElement* prefab_options = doc->NewElement("PrefabOptions");

				prefab_file->SetAttribute("name", obj.prefab.name.c_str());
				prefab_file->SetAttribute("dir", obj.prefab.dir.c_str());
				prefab_options->SetAttribute("save", obj.prefab.save);
				prefab_options->SetAttribute("load", obj.prefab.load_needed);
				prefab_options->SetAttribute("serialize", obj.prefab.can_serialize);

				prefab_element->InsertFirstChild(prefab_file);
				prefab_element->InsertEndChild(prefab_options);

				node->InsertEndChild(prefab_element);*/
			}
			//Dont load anything else if its a prefab since that prefab should have it all. 
			else {
				//---------- Transform ---------- 
				if (obj.flags & COMPONENT_TRANSFORM) {
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
				}

				//---------- Material ---------- 
				if (obj.flags & COMPONENT_MATERIAL) {
					XMLElement* material = doc->NewElement("Material");
					material->SetAttribute("ID", obj.material);
					node->InsertEndChild(material);
				}
				//---------- ObjectID ----------
				if (obj.type != ObjectType::NONE) {
					XMLElement* object = doc->NewElement("Object");
					object->SetAttribute("ID", (int)obj.type);
					node->InsertEndChild(object);
				}

				//---------- Collider ----------
				if (obj.flags & COMPONENT_COLIDER) {
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
				}
			}

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

		XMLElement* Serializer::LoadPrefab(const ObjectData& obj)
		{
			/*	Loads the prefab
			 *	Edits the transform
			 *	Returns the root of the file  */

			//Load teh file
			XMLDocument doc, copy;
			XMLError e_result = doc.LoadFile((obj.prefab.dir + obj.prefab.name + ".prefab").c_str());
			XMLCheckResult(e_result);
			doc.DeepCopy(&copy);

			XMLNode* p_node = copy.FirstChild();
			XMLElement* p_root_node = copy.FirstChildElement("Root");
			p_root_node = p_root_node->FirstChildElement("Node");

			//Search for the transform data
			XMLElement* transform = p_root_node->FirstChildElement("Transform");
			XMLElement* position = transform->FirstChildElement("Position");
			XMLElement* rotation = transform->FirstChildElement("Rotation");
			XMLElement* scale = transform->FirstChildElement("Scale");

			//Change the Transform Data
			position->SetAttribute("x", obj.pos.x);
			position->SetAttribute("y", obj.pos.y);
			position->SetAttribute("z", obj.pos.z);
			rotation->SetAttribute("x", obj.rot.x);
			rotation->SetAttribute("y", obj.rot.y);
			rotation->SetAttribute("z", obj.rot.z);
			   scale->SetAttribute("x", obj.size.x);
			   scale->SetAttribute("y", obj.size.y);
			   scale->SetAttribute("z", obj.size.z);
			//Return the root node
			return p_root_node;
		}

		//This is dumb but im leaving it here just incase it wasn't
		void Serializer::FixAllPrefabs(std::string dir)
		{
			XMLDocument doc;
			XMLError e_result;
			for (const auto& p : std::filesystem::directory_iterator(dir)) {
				e_result = doc.LoadFile(p.path().string().c_str());
				if (e_result == XML_SUCCESS) {
					XMLNode* root_node = doc.FirstChild();
					XMLElement* root = doc.FirstChildElement("Root");
					XMLElement* node = root->FirstChildElement("Node");

					//First set the flag to a component prefab flag
					int e_flags = 0;
					node->QueryIntAttribute("eFlags", &e_flags);
					if (!(e_flags & COMPONENT_PREFAB)) {
						e_flags |= COMPONENT_PREFAB;
						node->SetAttribute("eFlags", e_flags);

						//Then create a prefab component 
						XMLElement* prefab_element = doc.NewElement("Prefab");
						XMLElement* prefab_file = doc.NewElement("FilePath");
						XMLElement* prefab_options = doc.NewElement("PrefabOptions");

						auto stem = p.path().stem().string();
						const char* stem_name = stem.c_str();
						prefab_file->SetAttribute("name", stem_name);
						prefab_file->SetAttribute("dir", dir.c_str());
						prefab_options->SetAttribute("save", false);
						prefab_options->SetAttribute("load", true);
						prefab_options->SetAttribute("serialize", false);

						prefab_element->InsertFirstChild(prefab_file);
						prefab_element->InsertEndChild(prefab_options);

						//node->InsertEndChild(prefab_element);
						//node->InsertFirstChild(prefab_element);
						node->InsertAfterChild(node->FirstChildElement("Transform"), prefab_element);

					}
					e_result = doc.SaveFile(p.path().string().c_str());
					std::cout << p.path().filename() << "" << e_result;
				}
			}
		}
	}
}