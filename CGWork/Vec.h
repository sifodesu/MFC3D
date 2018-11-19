#pragma once
class Vec
{
public:
	float x;
	float y;
	float z;
	float w;

	Vec(float s = 0.0f);
	Vec(float x, float y, float z, float w = 1);
	~Vec();
	float operator[](int i) const;
	float& operator[](int i);
	Vec operator+ (const Vec& v) const;
	Vec operator* (float s) const;
	Vec operator* (const Vec& v) const;
};

