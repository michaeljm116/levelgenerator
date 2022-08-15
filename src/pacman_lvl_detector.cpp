#include "pacman_lvl_detector.h"

namespace principia {
	namespace lvlgen {
		BlacmanLvlConverter::BlacmanLvlConverter(const ColorGraph& graph)
		{
			GraphToLevel(graph);
		}

		BlacmanLvlConverter::~BlacmanLvlConverter()
		{
		}

		void BlacmanLvlConverter::GraphToLevel(const ColorGraph& graph)
		{
			level_size = glm::vec2(graph.size(), graph[0].size());
			_visited = Memo(graph.size(), std::vector<bool>(graph[0].size(), false));
			for (int r = 0; r < graph.size(); ++r) {
				for (int c = 0; c < graph[0].size(); ++c) {
					DetectObject(r, c, graph);
				}
			}
		}

		void BlacmanLvlConverter::DetectObject(int r, int c, const ColorGraph& graph)
		{
			if (_visited[r][c]) return;
			_visited[r][c] = true;
			auto color = graph[r][c];
			auto return_data = ImageData(kTypeNone, kSubTypeNone, vec2(1,1), vec2(c, r));
			
			switch (color)
			{
			case principia::lvlgen::kColorRed:
				return_data.type = kTypeEnemy;
				return_data.subtype = kSubTypeEnemy4;
				AddCharacter(return_data);
				break;
			case principia::lvlgen::kColorPurple:
				return_data.type = kTypeEnemy;
				return_data.subtype = kSubTypeEnemy2;
				AddCharacter(return_data);
				break;
			case principia::lvlgen::kColorBlue:
				return_data.type = kTypeWall;
				return_data.subtype = kSubTypeNone;
				return_data.size = DetectWallSize(r, c, graph);
				AddWall(return_data);
				break;
			case principia::lvlgen::kColorGreen:
				return_data.type = kTypePlayer;
				return_data.subtype = kSubTypePlayer1;
				AddCharacter(return_data);
				break;
			case principia::lvlgen::kColorYellow:
				return_data.type = kTypeDot;
				return_data.subtype = kSubTypeBigDot;
				AddFood(return_data);
				break;
			case principia::lvlgen::kColorOrange:
				return_data.type = kTypeEnemy;
				return_data.subtype = kSubTypeEnemy3;
				AddCharacter(return_data);
				break;
			case principia::lvlgen::kColorWhite:
				return_data.type = kTypeDot;
				return_data.subtype = kSubTypeSmallDot;
				AddFood(return_data);
				break;
			case principia::lvlgen::kColorBlack:
				break;
			case principia::lvlgen::kColorCyan:
				return_data.type = kTypeEnemy;
				return_data.subtype = kSubTypeEnemy1;
				AddCharacter(return_data);
				break;
			default:
				break;
			}
		}

		BlacmanLvl BlacmanLvlConverter::GetLvl()
		{
			std::vector<ObjectData> walls;
			std::vector<ObjectData> food;
			std::vector<ObjectData> characters;

			walls.reserve(_walls.size());
			food.reserve(_food.size());
			characters.reserve(_characters.size());

			for (int i = 0; i < _walls.size(); ++i) {
				ObjectData o = ObjectData("Blue");
				o.name = std::string("Wall(" + std::to_string(i) + ")");
				o.size = glm::vec3(_walls[i].size.x, 2, _walls[i].size.y);
				o.pos = glm::vec3(_walls[i].position.x, 0, _walls[i].position.y);
				o.type = ObjectType::BOX;
				o.col_type = CollisionType::Box;
				o.flags |= COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_PRIMITIVE | COMPONENT_COLIDER | COMPONENT_HEADNODE;
				ConvertObjectPosition(o);
				walls.emplace_back(o);
			}

			for (int i = 0; i < _food.size(); ++i) {
				ObjectData o;

				if (_food[i].subtype == kSubTypeBigDot) {
					o = ObjectData("Gold");
					o.name = "pill(" + std::to_string(i) + ")";
					o.size = glm::vec3(0.6f);
				}
				else {
					o = ObjectData("White");
					o.name = "food(" + std::to_string(i) + ")";
					o.size = glm::vec3(0.3f);
				}
				o.pos = glm::vec3(_food[i].position.x + 0.5f, 0, _food[i].position.y);
				o.type = ObjectType::SPHERE;
				o.col_type = CollisionType::Sphere;
				o.flags |= COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_PRIMITIVE | COMPONENT_COLIDER | COMPONENT_HEADNODE;
				ConvertObjectPosition(o);
				food.emplace_back(o);
			}

			for (int i = 0; i < _characters.size(); ++i) {
				ObjectData o;

				if (_characters[i].type == kTypePlayer) {
					o = ObjectData("SnakeGreen");
					o.name = "BlacMan";
					o.size = glm::vec3(0.8f);
				}
				else {
					o.name = "Enemy(" + std::to_string(i) + ")";
					o.size = glm::vec3(0.75f);
				}
				o.pos = glm::vec3(_characters[i].position.x, 1, _characters[i].position.y);
				o.type = ObjectType::BOX;
				o.col_type = CollisionType::Box;
				o.flags |= COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_PRIMITIVE | COMPONENT_COLIDER | COMPONENT_HEADNODE;
				ConvertObjectPosition(o);
				characters.emplace_back(o);
			}


			ObjectData floor = ObjectData("Black");
			floor.name = "Floor";
			floor.size = glm::vec3(level_size.x * 0.5f, 1, level_size.y * 0.5f);
			floor.pos = glm::vec3(level_size.x * 0.5f, -1, level_size.y * 0.5f);
			floor.col_type = CollisionType::Box;
			floor.type = ObjectType::BOX;
			floor.flags |= COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_PRIMITIVE | COMPONENT_COLIDER | COMPONENT_HEADNODE;

			return BlacmanLvl(walls, food, characters, floor);
		}

		void BlacmanLvlConverter::AddWall(const ImageData& d)
		{
			bool right_wall = d.size.x > 1;
			bool left_wall = d.size.y > 1;

			auto right_copy = d;
			auto left_copy = d;
			right_copy.size.y = 1;
			left_copy.size.x = 1;

			if(left_wall && right_wall){
				_walls.push_back(right_copy);
				left_copy.size.y--;
				left_copy.position.y++;
				_walls.push_back(left_copy);
			}
			else if(right_wall){
				_walls.push_back(right_copy);
			}
			else if (left_wall){
				_walls.push_back(left_copy);
			}
			else {
				_walls.push_back(d);
			}
		}

		vec2 BlacmanLvlConverter::DetectWallSize(int r, int c, const ColorGraph& graph)
		{
			vec2 size = vec2();
			size.x = CheckWallRight(r, c, graph);
			size.y = CheckWallDown(r, c, graph);

			return size;
		}
		int BlacmanLvlConverter::CheckWallDown(int r, int c, const ColorGraph& graph)
		{
			int size = 1;
			bool can_go_right = ((r + 1) < graph.size()) && (graph[r + 1][c] == kColorBlue);

			while (can_go_right) {
				size++;
				r++;
				_visited[r][c] = true;
				can_go_right = ((r + 1) < graph.size()) && (graph[r + 1][c] == kColorBlue);
			}
			return size;
		}
		int BlacmanLvlConverter::CheckWallRight(int r, int c, const ColorGraph& graph)
		{
			int size = 1;
			bool can_go_down = ((c + 1) < graph[0].size()) && (graph[r][c + 1] == kColorBlue);

			while (can_go_down) {
				size++;
				c++;
				_visited[r][c] = true;
				can_go_down = ((c + 1) < graph[0].size()) && (graph[r][c + 1] == kColorBlue);
			}
			return size;
		}
		void BlacmanLvlConverter::ConvertObjectPosition(ObjectData& obj)
		{
			auto extents = obj.size * 0.5f;
			auto center = obj.pos + extents;
			center.z = level_size.y - center.z;
			obj.pos = center;
			obj.size = extents;
		}
	}
}