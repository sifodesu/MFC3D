#pragma once
#include "Vec.h"
#include "Globals.h"

class mat2
{
public:
	vec2 M[2];

	mat2(float s = 1.0f) {
		M[0][0] = s;
		M[1][1] = s;
	}

	mat2(const vec2& v1, const vec2& v2) {
		M[0] = v1;
		M[1] = v2;
	}

	mat2(const mat2& m) {
		M[0] = m[0];
		M[1] = m[1];
	}

	mat2(const mat3& m) {
		M[0] = m[0];
		M[1] = m[1];
	}

	mat2(const mat4& m) {
		M[0] = m[0];
		M[1] = m[1];
	}

	mat2(const float a[2][2]) {
		M[0] = vec2(a[0][0], a[0][1]);
		M[1] = vec2(a[1][0], a[1][1]);
	}

	vec2& operator[] (int i) {
		return M[i];
	}

	const vec2& operator[] (int i) const {
		return M[i];
	}

	mat2 operator+ (const mat2& m) const {
		return mat2(M[0] + m[0], M[1] + m[1]);
	}

	mat2 operator- (const mat2& m) const {
		return mat2(M[0] - m[0], M[1] - m[1]);
	}

	mat2 operator* (float s) const {
		return mat2(M[0] * s, M[1] * s);
	}

	mat2 operator* (const mat2& m) const {
		mat2 n(0.0f);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					n[i][k] += M[i][j] * m[j][k];
				}
			}
		}
		return n;
	}

	vec2 operator* (const vec2& v) const {
		return vec2(
			M[0][0] * v[0] + M[0][1] * v[1],
			M[1][0] * v[0] + M[1][1] * v[1]
		);
	}

	mat2 operator/ (float s) const {
		return *this * (1.0f / s);
	}

	mat2 transpose() {
		return mat2({
			{M[0][0], M[1][0]}, 
			{M[0][1], M[1][1]}}
		);
	}
};

class mat3
{
public:
	vec3 M[3];

	mat3(float s = 1.0f) {
		M[0][0] = s;
		M[1][1] = s;
		M[2][2] = s;
	}

	mat3(const vec3& v1, const vec3& v2, const vec3& v3) {
		M[0] = v1;
		M[1] = v2;
		M[2] = v3;
	}

	mat3(const mat3& m) {
		M[0] = m[0];
		M[1] = m[1];
		M[2] = m[2];
	}

	mat3(const mat4& m) {
		M[0] = m[0];
		M[1] = m[1];
		M[2] = m[2];
	}

	mat3(const float a[3][3]) {
		M[0] = vec3(a[0][0], a[0][1], a[0][2]);
		M[1] = vec3(a[1][0], a[1][1], a[1][2]);
		M[2] = vec3(a[2][0], a[2][1], a[2][2]);
	}

	vec3& operator[] (int i) {
		return M[i];
	}

	const vec3& operator[] (int i) const {
		return M[i];
	}

	mat3 operator+ (const mat3& m) const {
		return mat3(M[0] + m[0], M[1] + m[1], M[2] + m[2]);
	}

	mat3 operator- (const mat3& m) const {
		return mat3(M[0] - m[0], M[1] - m[1], M[2] - m[2]);
	}

	mat3 operator* (float s) const {
		return mat3(M[0] * s, M[1] * s, M[2] * s);
	}

	mat3 operator* (const mat3& m) const {
		mat3 n(0.0f);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					n[i][k] += M[i][j] * m[j][k];
				}
			}
		}
		return n;
	}

	vec3 operator* (const vec3& v) const {
		return vec3(
			M[0][0] * v[0] + M[0][1] * v[1] + M[0][2] * v[2],
			M[1][0] * v[0] + M[1][1] * v[1] + M[1][2] * v[2],
			M[2][0] * v[0] + M[2][1] * v[1] + M[2][2] * v[2]
		);
	}

	mat3 operator/ (float s) const {
		return *this * (1.0f / s);
	}

	mat3 transpose() {
		return mat3({
			{M[0][0], M[1][0], M[2][0]},
			{M[0][1], M[1][1], M[2][1]},
			{M[0][2], M[1][2], M[2][2]}}
		);
	}
};

class mat4
{
public:
	vec4 M[4];

	mat4(float s = 1.0f) {
		M[0][0] = s;
		M[1][1] = s;
		M[2][2] = s;
		M[3][3] = s;
	}

	mat4(const vec4& v1, const vec4& v2, const vec4& v3, const vec4& v4) {
		M[0] = v1;
		M[1] = v2;
		M[2] = v3;
		M[3] = v3;
	}

	mat4(const mat4& m) {
		M[0] = m[0];
		M[1] = m[1];
		M[2] = m[2];
		M[3] = m[3];
	}

	mat4(const float a[4][4]) {
		M[0] = vec4(a[0][0], a[0][1], a[0][2], a[0][3]);
		M[0] = vec4(a[1][0], a[1][1], a[1][2], a[1][3]);
		M[0] = vec4(a[2][0], a[2][1], a[2][2], a[2][3]);
		M[0] = vec4(a[3][0], a[3][1], a[3][2], a[3][3]);
	}

	vec4& operator[] (int i) {
		return M[i];
	}

	const vec4& operator[] (int i) const {
		return M[i];
	}

	mat4 operator+ (const mat4& m) const {
		return mat4(M[0] + m[0], M[1] + m[1], M[2] + m[2], M[3] + m[3]);
	}

	mat4 operator- (const mat4& m) const {
		return mat4(M[0] - m[0], M[1] - m[1], M[2] - m[2], M[3] - m[3]);
	}

	mat4 operator* (float s) const {
		return mat4(M[0] * s, M[1] * s, M[2] * s, M[3] * s);
	}

	mat4 operator* (const mat4& m) const {
		mat4 n(0.0f);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					n[i][k] += M[i][j] * m[j][k];
				}
			}
		}
		return n;
	}

	vec4 operator* (const vec4& v) const {
		return vec4(
			M[0][0] * v[0] + M[0][1] * v[1] + M[0][2] * v[2] + M[0][3] * v[3],
			M[1][0] * v[0] + M[1][1] * v[1] + M[1][2] * v[2] + M[1][3] * v[3],
			M[2][0] * v[0] + M[2][1] * v[1] + M[2][2] * v[2] + M[2][3] * v[3],
			M[3][0] * v[0] + M[3][1] * v[1] + M[3][2] * v[2] + M[3][3] * v[3]
		);
	}

	mat4 operator/ (float s) const {
		return *this * (1.0f / s);
	}

	mat4 transpose() {
		return mat4({
			{M[0][0], M[1][0], M[2][0], M[3][0]},
			{M[0][1], M[1][1], M[2][1], M[3][1]},
			{M[0][2], M[1][2], M[2][2], M[3][2]},
			{M[0][3], M[1][3], M[2][3], M[3][3]}}
		);
	}
};

mat4 rotation_X(float angle) {
	float t = (PI / 180.0f) * angle;
	return mat4({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cos(t), -sin(t), 0.0f},
		{0.0f, sin(t), cos(t), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}}
	);
}

mat4 rotation_Y(float angle) {
	float t = (PI / 180.0f) * angle;
	return mat4({
		{cos(t), 0.0f, sin(t), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sin(t), 0.0f, cos(t), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}}
	);
}

mat4 rotation_Z(float angle) {
	float t = (PI / 180.0f) * angle;
	return mat4({
		{cos(t), -sin(t), 0.0f, 0.0f},
		{sin(t), cos(t), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}}
	);
}

mat4 translation(float x, float y, float z) {
	return mat4({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{x, y, z, 1.0f}}
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