#pragma once
#include "Mat.h"

class CCamera
{
public:
	mat4 projection;
	float d;
	float a;
	bool is_ortho;

	CCamera();
	CCamera(const CCamera& camera);
	void set_depth(float depth);
	void set_orthographic();
	void set_perspective();
	bool is_orthographic() const;
};

