#include "Polygon.h"

CPolygon::CPolygon() :
	has_normal(false)
{
}

CPolygon::CPolygon(const Vec & normal) :
	normal(normal), has_normal(true)
{
}

CPolygon::~CPolygon()
{
}

void CPolygon::add_vertice(const Vertice & vertice)
{
	vertices.push_back(vertice);
}
