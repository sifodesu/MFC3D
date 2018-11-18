#include "Polygon.h"

Polygon::Polygon() :
	has_normal(false)
{
}

Polygon::Polygon(const Vec & normal) :
	normal(normal), has_normal(true)
{
}

Polygon::~Polygon()
{
}

void Polygon::add_vertice(const Vertice & vertice)
{
	vertices.push_back(vertice);
}
