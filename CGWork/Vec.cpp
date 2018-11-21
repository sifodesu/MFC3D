#include "Vec.h"

float norm(const vec3& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float norm(const vec4& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec3 normalized(const vec3& v) {
	return v / norm(v);
}

vec4 normalized(const vec4& v) {
	return v / norm(v);
}

vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}
