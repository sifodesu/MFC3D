#pragma once
#include <vector>
#include "Vec.h"
using namespace std;

class CPolygon
{
public:
	vector<vec3> vertices;
	CPolygon();
	void add_vertice(const vec3& vertice);
};

