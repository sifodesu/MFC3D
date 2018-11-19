#pragma once
#include "Mat.h"

class Camera
{
public:
	Mat transform;
	Mat projection;

	Camera();
	~Camera();
};

