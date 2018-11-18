#pragma once
#include <vector>
#include "Vertice.h"

using namespace std;

class Polygon
{
public:
	vector<Vertice> vertices;
	Vec normal;
	bool has_normal;
	Polygon();
	Polygon(const Vec& normal);
	~Polygon();
	void add_vertice(const Vertice& vertice);
};

