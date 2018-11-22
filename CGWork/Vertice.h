#pragma once
#include "Vec.h"

class CVertice
{
	bool imported;

public:
	vec3 point;
	vec3 imported_normal;
	vec3 calculated_normal;

	CVertice(const vec3& point);
	CVertice(const vec3& point, const vec3& normal);
	bool has_imported_normal();
};

