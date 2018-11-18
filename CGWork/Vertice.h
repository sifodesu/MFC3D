#pragma once
#include "Vec.h"

class Vertice
{
public:
	Vec vertice;
	Vec normal;
	bool has_normal;
	Vertice(const Vec& vertice);
	Vertice(const Vec& vertice, const Vec& normal);
	~Vertice();
};

