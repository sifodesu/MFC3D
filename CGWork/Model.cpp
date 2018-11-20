#include "Model.h"

CModel::CModel()
{
}

void CModel::reset()
{
	polygons = vector<CPolygon>();
	transform = mat3();
	position = mat3();
}

void CModel::add_polygon(const CPolygon & polygon)
{
	polygons.push_back(polygon);
}

