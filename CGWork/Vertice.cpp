#include "Vertice.h"

Vertice::Vertice(const Vec & vertice) :
	vertice(vertice), has_normal(false)
{
}

Vertice::Vertice(const Vec & vertice, const Vec & normal) :
	vertice(vertice), normal(normal), has_normal(true)
{
}

Vertice::~Vertice()
{
}
