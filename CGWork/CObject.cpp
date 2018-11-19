#include "CObject.h"



ModelObject::ModelObject()
{
}


ModelObject::~ModelObject()
{
}

void ModelObject::add_polygon(const CPolygon& polygon)
{
	polygons.push_back(polygon);
}