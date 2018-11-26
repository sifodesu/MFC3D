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
	vec3 included_normal;
	CPolygon();
	bool highlight;
	//CPolygon(const CVertice& v1, const CVertice& v2, const CVertice& v3);
	//CPolygon(const CVertice& v1, const CVertice& v2, const CVertice& v3, const vec3& v4);
	void add_vertice(const CVertice& vertice);
};

