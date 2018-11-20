#pragma once
#include "Mat.h"

class Camera
{
public:
	mat3 transform;
	mat3 projection;

	Camera();
	~Camera();
};

