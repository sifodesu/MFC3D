#include "Polygon.h"

CPolygon::CPolygon() {}

CPolygon::CPolygon(const vec3 & v1, const vec3 & v2, const vec3 & v3)
{
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
}

CPolygon::CPolygon(const vec3 & v1, const vec3 & v2, const vec3 & v3, const vec3 & v4)
{
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
}

void CPolygon::add_vertice(const vec3& vertice)
{
	vertices.push_back(vertice);
}
