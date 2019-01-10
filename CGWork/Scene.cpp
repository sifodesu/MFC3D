#include "stdafx.h"
#include "CGWork.h"
#include "CGWorkView.h"
#include "PngWrapper.h"

CCGWorkView::CScene::CScene(CCGWorkView* parent) :
current_camera(0), renderer(parent), display_z_buffer(false), isBackgroundLoaded(false), isBackgroundStretched(true), lastFrame(nullptr), sizeLastFrame(0)
{
	cameras.push_back(CCamera());
	recording = false;
	playing = false;
}

void CCGWorkView::CScene::add_model(const CModel & model)
{
	models.push_back(model);
	active_model = models.size() - 1;
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
	case TRANSLATION:
		transformation = translation(x * TRANSLATION_FACTOR * app->mouse_sensitivity,
			y * TRANSLATION_FACTOR * app->mouse_sensitivity, z * TRANSLATION_FACTOR * app->mouse_sensitivity);
		break;
	case ROTATION:
		transformation = rotation(x * ROTATION_FACTOR * app->mouse_sensitivity,
			y * ROTATION_FACTOR * app->mouse_sensitivity, z * ROTATION_FACTOR * app->mouse_sensitivity);
		break;
	case SCALING:
		transformation = scaling(1 + x * SCALING_FACTOR * app->mouse_sensitivity,
			1 + y * SCALING_FACTOR * app->mouse_sensitivity, 1 + z * SCALING_FACTOR * app->mouse_sensitivity);
		break;
	}

	switch (app->transform_context) {
	case MODEL:
		models[active_model].transform_model(transformation, renderer.view_transform);
		break;
	case VIEW:
		renderer.view_transform = transformation * renderer.view_transform;
		for (CModel& model : models) {
			model.apply_transform(renderer.view_transform);
		}
		break;
	}
}

void CCGWorkView::CScene::drawZBuffer() {
	int h = renderer.screen.Height();
	int w = renderer.screen.Width();
	float maxx = renderer.z_buffer[0][0];
	float minn = renderer.z_buffer[0][0];
	for (auto y = 0; y < h; y++) {
		for (auto x = 0; x < w; x++) {
			if (maxx < renderer.z_buffer[y][x]) {
				maxx = renderer.z_buffer[y][x];
			}
			if (minn > renderer.z_buffer[y][x]) {
				minn = renderer.z_buffer[y][x];
			}
		}
	}
	for (auto y = 0; y < h; y++) {
		for (auto x = 0; x < w; x++) {
			if (renderer.bitFlag[x + y * w]) {
				float pp = 255.0f - ((renderer.z_buffer[y][x] - minn) * 255.0f / (maxx - minn));
				renderer.draw_pixel(POINT{ x, y }, RGB(pp, pp, pp));
			}
			else{
				renderer.draw_pixel(POINT{ x, y }, RGB(100, 100, 100));
			}
		}
	}
}

void CCGWorkView::CScene::draw_background() {
	float hW = renderer.screen.Height();
	float wW = renderer.screen.Width();
	float hI = background_image.GetHeight();
	float wI = background_image.GetWidth();
	int numChannels = background_image.GetNumChannels();

	if (!isBackgroundStretched) {
		int ch = 0; int cw = 0;
		while (ch < hW) {
			for (int y = 0; y < hI; ++y) {
				if (y + ch >= hW) break;
				cw = 0;
			redoX:
				for (int x = 0; x < wI; ++x) {
					if (x + cw >= wW) break;

					COLORREF col = RGB(GET_R(background_image.GetValue(x, y)), GET_G(background_image.GetValue(x, y)), GET_B(background_image.GetValue(x, y)));
					renderer.draw_pixel(POINT{ x + cw, y + ch }, col);
				}
				cw += wI;
				if (cw < wW)
					goto redoX;
			}
			ch += hI;
		}
	}

	else {
		float ratioX = wI / wW;
		float ratioY = hI / hW;

		for (int y = 0; y < hW; y++) {
			for (int x = 0; x < wW; x++) {
				int rawColorValue = background_image.GetValue(x*ratioX, y*ratioY);
				COLORREF col = RGB(GET_R(rawColorValue), GET_G(rawColorValue), GET_B(rawColorValue));
				renderer.draw_pixel(POINT{ x, y }, col);
			}
		}
	}

}

BYTE* bitmapBackup(BYTE* bitmap, int w, int h) {
	BYTE* res = new BYTE[w*h*sizeof(DWORD)];
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++){
			unsigned int offset = 4 * ((h - y) * w + x);
			if (offset >= h * w * sizeof(DWORD) || offset < 0 || x >= w || x < 0) {
				continue;
			}
			res[offset] = bitmap[offset];
			res[offset + 1] = bitmap[offset + 1];
			res[offset + 2] = bitmap[offset + 2];
		}
	}
	return res;
}

void CCGWorkView::CScene::drawMotionBlur() {
	int h = renderer.screen.Height();
	int w = renderer.screen.Width();
	if (lastFrame != nullptr && sizeLastFrame == w * h * sizeof(DWORD)) {
		double t = 0.25;
		for (int y = 0; y < h; y++){
			for (int x = 0; x < w; x++){
				unsigned int offset = 4 * ((h - y) * w + x);
				if (offset >= h * w * sizeof(DWORD) || offset < 0 || x >= w || x < 0) {
					continue;
				}
				int B = renderer.bitmap[offset];
				int G = renderer.bitmap[offset + 1];
				int R = renderer.bitmap[offset + 2];
				COLORREF pCur = RGB(R, G, B);
				B = lastFrame[offset];
				G = lastFrame[offset + 1];
				R = lastFrame[offset + 2];
				COLORREF pLF = RGB(R, G, B);

				COLORREF newCol = renderer.add(renderer.multiply(pCur, 1 - t), renderer.multiply(pLF, t));
				renderer.draw_pixel(POINT{ x, y }, newCol);

			}
		}
	}

	if (lastFrame != nullptr)
		delete[] lastFrame;
	//save the frame
	lastFrame = bitmapBackup(renderer.bitmap, w, h);
	sizeLastFrame = w*h*sizeof(DWORD);
}

void CCGWorkView::CScene::screenshot(CCGWorkView* parent, float width, float height) {

	TCHAR szFilters[] = _T("PNG Files (*.png)|*.png|All Files (*.*)|*.*||");

	CFileDialog dlg(FALSE, _T("png"), _T("file.png"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);

	if (dlg.DoModal() != IDOK) {
		return;
	}

	CString path = dlg.GetPathName();
	CT2A ascii(path);

	renderer.set_bitmap_dimensions(height, width);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned int offset = 4 * (y * width + x);
			renderer.bitmap[offset] = GetBValue(renderer.background_color);
			renderer.bitmap[offset + 1] = GetGValue(renderer.background_color);
			renderer.bitmap[offset + 2] = GetRValue(renderer.background_color);
		}
	}
	draw();

	PngWrapper res(ascii.m_psz, width, height);
	res.InitWritePng();


	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned int offset = 4 * (y * width + x);
			int B = renderer.bitmap[offset];
			int G = renderer.bitmap[offset + 1];
			int R = renderer.bitmap[offset + 2];

			res.SetValue(x, height - y - 1, SET_RGB(R, G, B));
		}
	}
	res.WritePng();
	parent->update_draw_bitmap(); // Reset the bitmap to screen size
	parent->Invalidate();
}

void CCGWorkView::CScene::draw_fog() {
	int h = renderer.screen.Height();
	int w = renderer.screen.Width();

	COLORREF fogColor = RGB(255, 0, 255);
	double minBoundFog = -0.5;
	double maxBoundFog = 0.5;
	double fogIntensity = 0.5;
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			if (renderer.bitFlag[x + y * w]) {
				unsigned int offset = 4 * ((h - y) * w + x);
				if (offset >= h * w * sizeof(DWORD) || offset < 0 || x >= w || x < 0) {
					return;
				}
				int B = renderer.bitmap[offset];
				int G = renderer.bitmap[offset + 1];
				int R = renderer.bitmap[offset + 2];
				COLORREF col = RGB(R, G, B);
				double f = abs(max(0, min(1, (renderer.z_buffer[y][x] - minBoundFog) / (maxBoundFog - minBoundFog))));
				f *= fogIntensity;
				col = renderer.add(renderer.multiply(col, (1 - f)), renderer.multiply(fogColor, f));

				renderer.draw_pixel(POINT{ x, y }, col);

			}
		}
	}
}

void CCGWorkView::CScene::draw()
{
	if (renderer.rendering_type != CRenderer::SQUELETON && isBackgroundLoaded) {
		draw_background();
	}

	int h = renderer.screen.Height();
	int w = renderer.screen.Width();


	renderer.z_buffer.clear();
	renderer.z_buffer.resize(h, std::vector<float>(w, 0));

	renderer.bitFlag.clear();
	renderer.bitFlag.resize(w*h, 0);

	for (CModel& model : models) {
		renderer.draw_model(model);
	}

	//draw_fog();

	if (display_z_buffer) {
		drawZBuffer();
	}

	//drawMotionBlur();

}