#pragma once
#include "Vec.h"

class Mat
{
public:
	Vec m[4];

	Mat(float s = 1.0f);
	Mat(const Vec& v1, const Vec& v2, const Vec& v3, const Vec& v4);
	~Mat();
	const Vec& operator[] (int i) const;
	Vec& operator[] (int i);
	Mat operator+ (const Mat& m) const;
	Mat operator* (float s) const;
	Mat operator* (const Mat& m) const;
};

