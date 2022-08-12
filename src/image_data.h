#pragma once
#include <assert.h>
namespace principia {
	namespace lvlgen{
		enum ImageType {
			kTypeNone,
			kTypeWall,
			kTypeDot,
			kTypePlayer,
			kTypeEnemy
		};

		enum ImageSubType {
			kSubTypeNone,
			kSubTypeSmallDot,
			kSubTypeBigDot,
			kSubTypePlayer1,
			kSubTypePlayer2,
			kSubTypeEnemy1,
			kSubTypeEnemy2,
			kSubTypeEnemy3,
			kSubTypeEnemy4
		};

		struct vec2 {
			float x = 0, y = 0;
			vec2(float _x, float _y) : x(_x), y(_y) {};
			float& operator[](int i) {
				assert(i > -1 && i < 2);
				return *(&x + i);
			}
			vec2() {};
			vec2 operator+(const vec2& v) {
				return vec2(v.x + x, v.y + y);
			}
			vec2 operator-(const vec2& v) {
				return vec2(x - v.x, y - v.y);
			}
			vec2 operator*(const vec2& v) {
				return vec2(x * v.x, y * v.y);
			}
			vec2 operator/(const vec2& v) {
				return vec2(x / v.x, y / v.y);
			}
		};

		struct vec3 {
			float x = 0, y = 0, z = 0;
			vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
			float& operator[](int i) {
				assert(i > -1 && i < 3);
				return *(&x + i);
			}
			vec3 operator+(const vec3& v) {
				return vec3(v.x + x, v.y + y, v.z + z);
			}
			vec3 operator-(const vec3& v) {
				return vec3(x - v.x, y - v.y, z - v.z);
			}
			vec3 operator*(const vec3& v) {
				return vec3(x * v.x, y * v.y, z * v.z);
			}
			vec3 operator/(const vec3& v) {
				return vec3(x / v.x, y / v.y, z / v.z);
			}

		};

		struct ImageData {
			ImageType type;
			ImageSubType subtype;
			vec2 size;
			vec2 position;

			ImageData(ImageType t, ImageSubType st, vec2 s, vec2 p) :
				type(t), subtype(st), size(s), position(p) {};
			ImageData() {
				type = kTypeNone; subtype = kSubTypeNone;
				size = vec2(); position = vec2();
			};
		};
	}
}
