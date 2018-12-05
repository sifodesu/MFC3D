#include "stdafx.h"
#include "CGWork.h"
#include "CGWorkView.h"


CCGWorkView::CScene::CScene(CCGWorkView* parent) :
	current_camera(0), renderer(parent)
{
	cameras.push_back(CCamera());
}

void CCGWorkView::CScene::add_model(const CModel & model)
{
	models.clear();
	models.push_back(model);
}

void CCGWorkView::CScene::add_camera(const CCamera & camera)
{
}

void CCGWorkView::CScene::update_projection(int projection_type)
{
	for (CCamera& camera : cameras) {
		if (projection_type == ID_VIEW_ORTHOGRAPHIC) {
			camera.set_orthographic();
		}
		else {
			camera.set_perspective();
		}
	}
}

CCamera & CCGWorkView::CScene::get_current_camera()
{
	return cameras[current_camera];
}

void CCGWorkView::CScene::update(CCGWorkView* app, int mouse_dx)
{
	if (mouse_dx == 0) {
		return;
	}

	mat4 transformation;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	if (app->X) {
		x = (float)mouse_dx;
	}
	if (app->Y) {
		y = (float)mouse_dx;
	}
	if (app->Z) {
		z = (float)mouse_dx;
	}

	switch (app->m_nAction) {
	case ID_ACTION_TRANSLATE:
		transformation = translation(x * TRANSLATION_FACTOR * app->mouse_sensitivity,
			y * TRANSLATION_FACTOR * app->mouse_sensitivity, z * TRANSLATION_FACTOR * app->mouse_sensitivity);
		break;
	case ID_ACTION_ROTATE:
		transformation = rotation(x * ROTATION_FACTOR * app->mouse_sensitivity,
			y * ROTATION_FACTOR * app->mouse_sensitivity, z * ROTATION_FACTOR * app->mouse_sensitivity);
		break;
	case ID_ACTION_SCALE:
		transformation = scaling(1 + x * SCALING_FACTOR * app->mouse_sensitivity,
			1 + y * SCALING_FACTOR * app->mouse_sensitivity, 1 + z * SCALING_FACTOR * app->mouse_sensitivity);
		break;
	}

	for (CModel& model : models) {
		switch (app->transform_context) {
		case TRANSFORM_MODEL:
			model.transform_model(transformation);
			break;
		case TRANSFORM_VIEW:
			model.transform_view(transformation);
			break;
		}
	}
}

void CCGWorkView::CScene::draw(CDC* context)
{
	renderer.get_bitmap(context);
	renderer.set_camera(cameras[current_camera]);
	renderer.bitFlag.reset();
	
	int h = renderer.screen.Height();
	int w = renderer.screen.Width();
	renderer.z_buffer.resize(h, std::vector<float>(w, 0));

	for (CModel& model : models) {
		renderer.draw_model(model);
	}
	renderer.draw_bitmap(context);
}