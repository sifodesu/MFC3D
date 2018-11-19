#include "Vertice.h"

Vertice::Vertice(const Vec & vertice) :
	point(vertice), has_normal(false)
{
}

Vertice::Vertice(const Vec & vertice, const Vec & normal) :
	point(vertice), normal(normal), has_normal(true)
{
}

Vertice::~Vertice()
{
}
