#include "Polygon.h"

CPolygon::CPolygon() {}

void CPolygon::add_vertice(const CVertice& vertice)
{
	vertices.push_back(vertice);
}

void CPolygon::set_origin()
{
	vec3 new_origin;
	for (const CVertice& vertice : vertices) {
		new_origin = new_origin + vertice.transformed;
	}
	origin = new_origin / (float)vertices.size();
}
