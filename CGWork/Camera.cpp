#include "Camera.h"

CCamera::CCamera() :
	d(1.0f),
	a(0.25f),
	is_ortho(true)
{
	set_orthographic();
}

CCamera::CCamera(const CCamera & camera) :
	d(camera.d),
	a(camera.a),
	is_ortho(camera.is_ortho)
{
	if (is_ortho) {
		set_orthographic();
	}
	else {
		set_perspective();
	}
}

void CCamera::set_depth(float depth)
{
	d = depth;
	a = 0.25f * d;
}

void CCamera::set_orthographic()
{
	/*mat4 T = translation(
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
	projection = T * S * M;*/
	projection = mat4({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}}
	);
	is_ortho = true;
}

void CCamera::set_perspective()
{
	mat4 T = translation(0.0f, 0.0f, d - a);
	mat4 N({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, d / (d - a), 1 / d},
		{0.0f, 0.0f, -(d * a) / (d - a), 0.0f} }
	);
	projection = T * N;
	is_ortho = false;
}

bool CCamera::is_orthographic() const
{
	return is_ortho;
}
