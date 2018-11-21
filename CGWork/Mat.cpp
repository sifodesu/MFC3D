#include "Mat.h"

mat4 rotation_X(float angle) {
	float t = (PI / 180.0f) * angle;
	return mat4({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cos(t), -sin(t), 0.0f},
		{0.0f, sin(t), cos(t), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f} }
	);
}

mat4 rotation_Y(float angle) {
	float t = (PI / 180.0f) * angle;
	return mat4({
		{cos(t), 0.0f, sin(t), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sin(t), 0.0f, cos(t), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f} }
	);
}

mat4 rotation_Z(float angle) {
	float t = (PI / 180.0f) * angle;
	return mat4({
		{cos(t), -sin(t), 0.0f, 0.0f},
		{sin(t), cos(t), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f} }
	);
}

mat4 translation(float x, float y, float z) {
	return mat4({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{x, y, z, 1.0f} }
	);
}

mat4 scaling(float x, float y, float z) {
	return mat4({
		{x, 0.0f, 0.0f, 0.0f},
		{0.0f, y, 0.0f, 0.0f},
		{0.0f, 0.0f, z, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f} }
	);
}