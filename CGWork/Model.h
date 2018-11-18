#pragma once
#include <vector>
#include "Polygon.h"
#include "Mat.h"

using namespace std;

class Model
{
public:
	vector<Polygon> polygons;
	Mat transform;

	Model();
	~Model();
	std::vector<Polygon> polygons;
	void addPolygon();
};

