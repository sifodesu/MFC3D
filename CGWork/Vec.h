#pragma once
#include <cmath>
#include <functional>
using std::hash;

using namespace std;

const int p = 100000;


class vec4
{
public:
	float x;
	float y;
	float z;
	float w;

	vec4(float s = 0.0f) :
		x(s), y(s), z(s), w(s) {}

	vec4(float x, float y, float z, float w) :
		x(x), y(y), z(z), w(w) {}

	vec4(const vec4& v) :
		x(v.x), y(v.y), z(v.z), w(v.w) {}

	float& operator[] (int i) {
		return *(&x + i);
	}

	const float& operator[] (int i) const {
		return *(&x + i);
	}

	vec4 operator+ (const vec4& v) const {
		return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	vec4 operator- () const {
		return vec4(-x, -y, -z, -w);
	}

	vec4 operator- (const vec4& v) const {
		return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	vec4 operator* (float s) const {
		return vec4(x * s, y * s, z * s, w * s);
	}

	vec4 operator* (const vec4& v) const {
		return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	vec4 operator/ (float s) const {
		return *this * (1.0f / s);
	}

	//bool operator== (const vec4& v) const {
	//	return ((int)this->x*p == (int)v.x*p) && ((int)this->y*p == (int)v.y*p)
	//		&& ((int)this->z*p == (int)v.z*p) && ((int)this->w*p == (int)v.w*p);
	//}
};

class vec3
{
public:
	float x;
	float y;
	float z;

	vec3(float s = 0.0f) :
		x(s), y(s), z(s) {}

	vec3(float x, float y, float z) :
		x(x), y(y), z(z) {}

	vec3(const vec3& v) :
		x(v.x), y(v.y), z(v.z) {}

	vec3(const vec4& v) :
		x(v.x), y(v.y), z(v.z) {}

	float& operator[] (int i) {
		return *(&x + i);
	}

	const float& operator[] (int i) const {
		return *(&x + i);
	}

	vec3 operator+ (const vec3& v) const {
		return vec3(x + v.x, y + v.y, z + v.z);
	}

	vec3 operator- () const {
		return vec3(-x, -y, -z);
	}

	vec3 operator- (const vec3& v) const {
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	vec3 operator* (float s) const {
		return vec3(x * s, y * s, z * s);
	}

	vec3 operator* (const vec3& v) const {
		return vec3(x * v.x, y * v.y, z * v.z);
	}

	vec3 operator/ (float s) const {
		return *this * (1.0f / s);
	}

	bool operator== (const vec3& v) const {
		return ((int)(this->x*p) == (int)(v.x*p)) && ((int)(this->y*p) == (int)(v.y*p))
			&& ((int)(this->z*p) == (int)(v.z*p));
	}

};

class vec2
{
public:
	float x;
	float y;

	vec2(float s = 0.0f) :
		x(s), y(s) {}

	vec2(float x, float y) :
		x(x), y(y) {}

	vec2(const vec2& v) :
		x(v.x), y(v.y) {}

	vec2(const vec3& v) :
		x(v.x), y(v.y) {}

	vec2(const vec4& v) :
		x(v.x), y(v.y) {}

	float& operator[] (int i) {
		return *(&x + i);
	}

	const float& operator[] (int i) const {
		return *(&x + i);
	}

	vec2 operator+ (const vec2& v) const {
		return vec2(x + v.x, y + v.y);
	}

	vec2 operator- () const {
		return vec2(-x, -y);
	}

	vec2 operator- (const vec2& v) const {
		return vec2(x - v.x, y - v.y);
	}

	vec2 operator* (float s) const {
		return vec2(x * s, y * s);
	}

	vec2 operator* (const vec2& v) const {
		return vec2(x * v.x, y * v.y);
	}

	vec2 operator/ (float s) const {
		return *this * (1.0f / s);
	}

	bool operator== (const vec2& v) const {
		return ((int)(this->x*p) == (int)(v.x*p)) && ((int)(this->y*p) == (int)(v.y*p));
	}
};

float norm(const vec3& v);
float norm(const vec4& v);
vec3 normalized(const vec3& v);
vec4 normalized(const vec4& v);
vec3 cross(const vec3& v1, const vec3& v2);

class edge {
public:
	vec3 first;
	vec3 second;
	edge(vec2 first, vec2 second) {
		this->first = first.x < second.x ? vec3(first.x, first.y, 0) : vec3(second.x, second.y, 0);
		this->second = this->first == vec3(first.x, first.y, 0) ? vec3(second.x, second.y, 0) : vec3(first.x, first.y, 0);
	}
	bool operator==(const edge& e) const {
		return this->first == e.first && this->second == e.second;
	}
};



namespace std
{
	template <>
	struct hash<vec3>
	{
		size_t operator()(const vec3& k) const
		{
			return ((hash<int>()((int)(k.x*p))
				^ (hash<int>()((int)(k.y*p)) << 1)) >> 1)
				^ (hash<int>()((int)(k.z*p)) << 1);
		}
	};

	template <>
	struct hash<edge>
	{
		size_t operator()(const edge& k) const
		{
			return ((hash<vec3>()(k.first)
				^ (hash<vec3>()(k.second) << 1)));
		}
	};
}
//
//struct vec3Hasher
//{
//	size_t operator()(const vec3& k) const
//	{
//		return ((std::hash<float>()(k.x)
//			^ (std::hash<float>()(k.y) << 1)) >> 1)
//			^ (std::hash<float>()(k.z) << 1);
//	}
//};