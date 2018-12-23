#pragma once
#include <vector>
#include "StdAfx.h"
#include "Polygon.h"
#include "Mat.h"
#include "Globals.h"
#include <unordered_set>
#include <unordered_map>

using namespace std;

class CModel
{
public:
	vector<CPolygon> polygons;
	vector<vec3> bounding_box;

	mat4 model_transform;
	mat4 view_transform;
	mat4 position;

	COLORREF color;
	COLORREF normalsColor;

	CModel(COLORREF color = WHITE, COLORREF normalsColor = PINK);
	void setup_model();
	void calculate_normals();

	void add_polygon(const CPolygon& polygon);

	void apply_transform();

	void transform_model(const mat4& m);
	void transform_view(const mat4& m);
};

