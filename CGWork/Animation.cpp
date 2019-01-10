#include "Animation.h"
#include "Globals.h"

Animation::Animation()
{
	speed = 30;
	current_step = 0;
	next_keyframe = 0;
	acc = 0;
	restart = false;
}

void Animation::start_recording(const mat4& model, const mat4& view)
{
	keyframes.clear();
	start_model_mat = model;
	start_view_mat = view;
	current_step = 0;
	next_keyframe = 0;
	acc = 0;
}

void Animation::add_keyframe(bool x, bool y, bool z, int transform, int space)
{
	keyframes.push_back(KeyFrame(acc, x, y, z, transform, space));
	acc = 0;
}

mat4 Animation::get_transform(bool* is_view, float sensitivity)
{
	mat4 transformation;
	KeyFrame current = keyframes[next_keyframe];
	float diff = current.acc / (float)speed;
	float x = (current.x) ? diff : 0.0f;
	float y = (current.y) ? diff : 0.0f;
	float z = (current.z) ? diff : 0.0f;

	switch (current.transform_type) {
	case TRANSLATION:
		transformation = translation(
			x * TRANSLATION_FACTOR * sensitivity,
			y * TRANSLATION_FACTOR * sensitivity,
			z * TRANSLATION_FACTOR * sensitivity);
		break;
	case ROTATION:
		transformation = rotation(
			x * ROTATION_FACTOR * sensitivity,
			y * ROTATION_FACTOR * sensitivity, 
			z * ROTATION_FACTOR * sensitivity);
		break;
	case SCALING:
		transformation = scaling(
			1 + x * SCALING_FACTOR * sensitivity,
			1 + y * SCALING_FACTOR * sensitivity, 
			1 + z * SCALING_FACTOR * sensitivity);
		break;
	}
	*is_view = current.space_type == VIEW;

	current_step++;
	if (current_step == speed) {
		current_step = 0;
		next_keyframe++;
		if (next_keyframe == keyframes.size()) {
			next_keyframe = 0;
			restart = true;
		}
	}

	return transformation;
}

KeyFrame::KeyFrame(float acc, bool x, bool y, bool z, int transform, int space) :
	acc(acc), x(x), y(y), z(z), transform_type(transform), space_type(space) {}
