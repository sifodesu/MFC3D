#pragma once
#include <vector>
#include "Vec.h"
#include "Vertice.h"
#include <map>

using namespace std;

class CPolygon
{
public:
	vector<CVertice> vertices;
	vec3 origin_transformed;
	vec3 origin;
	vec3 included_normal;
	vec3 calculated_normal;
	CPolygon();
	void add_vertice(const CVertice& vertice);
	void set_origin();
	void set_origin_transformed();
};

