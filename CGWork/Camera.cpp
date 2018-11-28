#include "Camera.h"

CCamera::CCamera() :
	position(0, 0, 0, 1),
	top(1.0f), bottom(-1.0f),
	left(-1.0f), right(1.0f),
	_near(-1.0f), _far(1.0f),
	is_ortho(true)
{
	set_orthographic();
}

CCamera::CCamera(const CCamera & camera) :
	position(camera.position),
	top(camera.top), bottom(camera.bottom),
	left(camera.left), right(camera.right),
	_near(camera._near), _far(camera._far),
	is_ortho(camera.is_ortho)
{
	if (is_ortho) {
		set_orthographic();
	}
	else {
		set_perspective();
	}
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
	projection = T * S * M;
	is_ortho = true;
}

void CCamera::set_perspective()
{
	mat4 T = translation(
		-(left + right) / 2.0f,
		-(bottom + top) / 2.0f,
		(_near + _far) / 2.0f + 1.0f
	);
	mat4 N({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, _far / (_far - _near), 1 / _far},
		{0.0f, 0.0f, -(_far * _near) / (_far - _near), 0.0f}}
	);
	projection = T * N;
	is_ortho = false;
}

bool CCamera::is_orthographic() const
{
	return is_ortho;
}
