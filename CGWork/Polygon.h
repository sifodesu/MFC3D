#pragma once
#include <vector>
#include "Vertice.h"

using namespace std;

class CPolygon
{
public:
	vector<Vertice> vertices;
	Vec normal;
	bool has_normal;
	CPolygon();
	CPolygon(const Vec& normal);
	~CPolygon();
	void add_vertice(const Vertice& vertice);
};

