#include "Camera.h"

CCamera::CCamera() :
	d(2.0f),
	a(0.0f),
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
	d = depth / DEPTH_FACTOR;
	if (!is_ortho) {
		set_perspective();
	}
}

void CCamera::set_orthographic()
{
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
