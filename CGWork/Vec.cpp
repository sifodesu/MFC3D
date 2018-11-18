#include "Vec.h"
#include <exception>

using namespace std;

Vec::Vec(float s) :
	x(s), y(s), z(s), w(s)
{
}

Vec::Vec(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{
}

Vec::~Vec()
{
}

float Vec::operator[](int i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	if (i == 3) return w;
	throw new exception();
}

float & Vec::operator[](int i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	if (i == 3) return w;
	throw new exception();
}

Vec Vec::operator+(const Vec & v) const
{
	return Vec(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vec Vec::operator*(float s) const
{
	return Vec(x * s, y * s, z * s, w * s);
}

Vec Vec::operator*(const Vec & v) const
{
	return Vec(x * v.x, y * v.y, z * v.z, w * v.w);
}
