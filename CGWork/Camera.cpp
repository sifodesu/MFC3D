#include "Camera.h"

CCamera::CCamera() :
	position(0, 0, 0, 1),
	top(10.0f), bottom(-10.0f),
	left(-10.0f), right(10.0f),
	_near(-10.0f), _far(10.0f),
	is_ortho(true)
{
	set_orthographic();
}

void CCamera::set_orthographic()
{
	mat4 T = translation(
		-(left + right) / 2.0f,
		-(bottom + top) / 2.0f,
		(_near + _far) / 2.0f
	);
	mat4 S = scaling(
		2.0f / (right - left),
		2.0f / (top - bottom),
		2.0f / (_near - _far)
	);
	mat4 M = scaling(1.0f, 1.0f, 0.0f);
	projection = M * S * T;
}

bool CCamera::is_orthographic()
{
	return is_ortho;
}
