#pragma once

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
};

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
};