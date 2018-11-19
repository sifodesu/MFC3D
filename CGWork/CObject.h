#pragma once
#include <vector>
#include "Polygon.h"

using namespace std;

class ModelObject
{
public:
	vector<CPolygon> polygons;

	ModelObject();
	~ModelObject();
	void add_polygon(const CPolygon& polygon);
};

