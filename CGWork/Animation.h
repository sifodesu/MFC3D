#pragma once
#include <vector>
#include "Mat.h"

using std::vector;

class KeyFrame
{
public:
	float acc;
	bool x, y, z;
	int transform_type;
	int space_type;

	KeyFrame(float acc, bool x, bool y, bool z, int transform, int space);
};

class Animation
{
public:
	vector<KeyFrame> keyframes;
	int speed;
	mat4 start_model_mat;
	mat4 start_view_mat;

	int acc;
	int current_step;
	int next_keyframe;
	bool restart;

	Animation();
	void start_recording(const mat4& model, const mat4& view);
	void add_keyframe(bool x, bool y, bool z, int transform, int space);
	mat4 get_transform(bool* is_view, float sensitivity);
};

