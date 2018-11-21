#pragma once
#include <vector>
#include "Vec.h"
using namespace std;

class CPolygon
{
public:
	vector<vec3> vertices;
	CPolygon();
	CPolygon(const vec3& v1, const vec3& v2, const vec3& v3);
	CPolygon(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& v4);
	void add_vertice(const vec3& vertice);
};

