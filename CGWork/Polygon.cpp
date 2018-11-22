#include "Polygon.h"

CPolygon::CPolygon() {}

//CPolygon::CPolygon(const CVertice & v1, const CVertice & v2, const CVertice & v3)
//{
//	vertices.push_back(v1);
//	vertices.push_back(v2);
//	vertices.push_back(v3);
//}
//
//CPolygon::CPolygon(const CVertice & v1, const CVertice & v2, const CVertice & v3, const CVertice & v4)
//{
//	vertices.push_back(v1);
//	vertices.push_back(v2);
//	vertices.push_back(v3);
//	vertices.push_back(v4);
//}

void CPolygon::add_vertice(const CVertice& vertice)
{
	vertices.push_back(vertice);
}
