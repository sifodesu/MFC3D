#include "Polygon.h"

CPolygon::CPolygon() {}

void CPolygon::add_vertice(const vec3& vertice)
{
	vertices.push_back(vertice);
}
