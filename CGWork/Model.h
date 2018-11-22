#pragma once
#include <vector>
#include "StdAfx.h"
#include "Polygon.h"
#include "Mat.h"

using namespace std;

class CModel
{
public:
	vector<CPolygon> polygons;
	vector<vec3> bounding_box;
	//vector<vec3> included_normals;

	mat4 model_transform;
	mat4 view_transform;
	mat4 position;
	COLORREF color;
	COLORREF normalsColor;

	CModel(COLORREF color = RGB(255, 255, 255), COLORREF normalsColor = RGB(255, 50 ,50));
	void set_bounding_box();
	void add_polygon(const CPolygon& polygon);
	//void add_included_normal(const vec3 normal);
};

