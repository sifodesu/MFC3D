#pragma once
#include <vector>
#include "Polygon.h"
#include "Mat.h"

using namespace std;

class CModel
{
public:
	vector<CPolygon> polygons;
	mat3 transform;
	mat3 position;

	CModel();
	void reset();
	void add_polygon(const CPolygon& polygon);
};

