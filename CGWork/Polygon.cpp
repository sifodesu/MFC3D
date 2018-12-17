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
		new_origin = new_origin + vertice.point;
	}
	origin_transformed = new_origin / (float)vertices.size();
}

void CPolygon::set_origin_transformed()
{
	vec3 new_origin_transformed;
	for (const CVertice& vertice : vertices) {
		new_origin_transformed = new_origin_transformed + vertice.transformed;
	}
	origin_transformed = new_origin_transformed / (float)vertices.size();
}
