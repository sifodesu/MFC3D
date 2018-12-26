#include "stdafx.h"

#include "CGWorkView.h"

CCGWorkView::CRenderer::CRenderer(CCGWorkView* parent) :
parent(parent), bitmap(nullptr),
rendering_type(SQUELETON),
shading_type(FLAT),
background_color(BLACK),
highlight_polygon(GREEN),
normals_color(PINK),
wireframe_color(WHITE),
bbox_color(RED)
{
	draw_bbox = false;
	draw_polygon_normals = false;
	draw_vertice_normals = false;
	invert_polygon_normals = false;
	invert_vertice_normals = false;
	draw_polygon_included_normals = true;
	draw_vertice_included_normals = true;
	select_highlighted_pol = false;
	backface_culling = false;

	// Init light default
	ambiant.intensity = 0.2;
	ambiant.exponent = 10.0f;
	lights[0].enabled = true;
	lights[0].type = LIGHT_POINT;
	lights[0].data = vec3(10.0f, 10.0f, -10.0f);
	lights[0].diffuse = 0.5;
	lights[0].specular = 0.5;
}

CCGWorkView::CRenderer::~CRenderer()
{
	if (bitmap != nullptr) {
		delete[] bitmap;
	}
}

void CCGWorkView::CRenderer::draw_pixel(POINT p, COLORREF c)
{
	unsigned int offset = 4 * ((screen.Height() - p.y) * screen.Width() + p.x);
	if (offset >= parent->BMInfo.bmiHeader.biSizeImage || offset < 0 || p.x >= screen.Width() || p.x < 0) {
		return;
	}
	bitmap[offset] = GetBValue(c);
	bitmap[offset + 1] = GetGValue(c);
	bitmap[offset + 2] = GetRValue(c);
}

//approximate the z of a point on a line
float get_approx_z(POINT p, const vec3& source, const vec3& dest) {
	float dx = dest.x - source.x;
	float dy = dest.y - source.y;
	if (dx < 1.0f && dx > -1.0f) {
		if (dy < 1.0f && dy > -1.0f) {
			return source.z;
		}
	}
	float t = (std::abs(dx) > std::abs(dy)) ? (p.x - source.x) / dx : (p.y - source.y) / dy;
	return source.z + t * (dest.z - source.z);
}

void CCGWorkView::CRenderer::set_pixel(POINT p, const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint) {
	if (p.x >= 0 && p.y >= 0 && p.x < min(3840, screen.Width()) && p.y < min(2160, screen.Height())) {
		if (!bitFlag.test(p.x + p.y * 3840)) {
			bitFlag.set(p.x + p.y * 3840);
			z_buffer[p.y][p.x] = get_approx_z(p, v1, v2);
			draw_pixel(p, color);
		}
		else {
			if (z_buffer[p.y][p.x] > get_approx_z(p, v1, v2)) {
				z_buffer[p.y][p.x] = get_approx_z(p, v1, v2);
				draw_pixel(p, color);
			}
			else if (forcePrint) {
				draw_pixel(p, color);
			}
		}
	}
}

void CCGWorkView::CRenderer::draw_line(const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint)
{
	int x1 = (int)v1.x, x2 = (int)v2.x, y1 = (int)v1.y, y2 = (int)v2.y;
	if (x2 - x1 < 0) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int dx = x2 - x1, dy = y2 - y1, d, delta_e, delta_ne;

	POINT p;
	p.x = x1;
	p.y = y1;

	set_pixel(p, v1, v2, color, forcePrint);


	if (dy >= 0) {
		if (dx >= dy) {
			d = 2 * dy - dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy - dx);
			while (p.x < x2) {
				if (d < 0) {
					d += delta_e;
					p.x++;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y++;
				}
				set_pixel(p, v1, v2, color, forcePrint);
			}
		}
		else {
			d = 2 * dx - dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx - dy);
			while (p.y < y2) {
				if (d < 0) {
					d += delta_e;
					p.y++;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y++;
				}
				set_pixel(p, v1, v2, color, forcePrint);

			}
		}
	}
	else {
		if (dx >= -dy) {
			d = 2 * dy + dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy + dx);
			while (p.x < x2) {
				if (d > 0) {
					d += delta_e;
					p.x++;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y--;
				}
				set_pixel(p, v1, v2, color, forcePrint);
			}
		}
		else {
			d = 2 * dx + dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx + dy);
			while (p.y > y2) {
				if (d < 0) {
					d += delta_e;
					p.y--;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y--;
				}
				set_pixel(p, v1, v2, color, forcePrint);
			}
		}
	}
}

void CCGWorkView::CRenderer::calculate_left(const vec3 & v1, const vec3 & v2, vector<pair<int, float>>& points, int min_y)
{
	int x1 = (int)v1.x, x2 = (int)v2.x, y1 = (int)v1.y, y2 = (int)v2.y;
	if (x2 - x1 < 0) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int dx = x2 - x1, dy = y2 - y1, d, delta_e, delta_ne;
	int x = x1, y = y1;

	points[y - min_y].first = x;
	points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);

	if (dy >= 0) {
		if (dx >= dy) {
			d = 2 * dy - dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy - dx);
			while (x < x2) {
				if (d < 0) {
					d += delta_e;
					x++;
				}
				else {
					d += delta_ne;
					x++;
					y++;
					points[y - min_y].first = x;
					points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
				}
			}
		}
		else {
			d = 2 * dx - dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx - dy);
			while (y < y2) {
				if (d < 0) {
					d += delta_e;
					y++;
				}
				else {
					d += delta_ne;
					x++;
					y++;
				}
				points[y - min_y].first = x;
				points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
			}
		}
	}
	else {
		if (dx >= -dy) {
			d = 2 * dy + dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy + dx);
			while (x < x2) {
				if (d > 0) {
					d += delta_e;
					x++;
				}
				else {
					d += delta_ne;
					x++;
					y--;
					points[y - min_y].first = x;
					points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
				}
			}
		}
		else {
			d = 2 * dx + dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx + dy);
			while (y > y2) {
				if (d < 0) {
					d += delta_e;
					y--;
				}
				else {
					d += delta_ne;
					x++;
					y--;
				}
				points[y - min_y].first = x;
				points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
			}
		}
	}
}

void CCGWorkView::CRenderer::calculate_right(const vec3 & v1, const vec3 & v2, vector<pair<int, float>>& points, int min_y)
{
	int x1 = (int)v1.x, x2 = (int)v2.x, y1 = (int)v1.y, y2 = (int)v2.y;
	if (x2 - x1 < 0) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int dx = x2 - x1, dy = y2 - y1, d, delta_e, delta_ne;
	int x = x1, y = y1;

	if (dy >= 0) {
		if (dx >= dy) {
			d = 2 * dy - dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy - dx);
			while (x < x2) {
				if (d < 0) {
					d += delta_e;
					x++;
				}
				else {
					d += delta_ne;
					points[y - min_y].first = x;
					points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
					x++;
					y++;
				}
			}
			points[y - min_y].first = x;
			points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
		}
		else {
			points[y - min_y].first = x;
			points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
			d = 2 * dx - dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx - dy);
			while (y < y2) {
				if (d < 0) {
					d += delta_e;
					y++;
				}
				else {
					d += delta_ne;
					x++;
					y++;
				}
				points[y - min_y].first = x;
				points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
			}
		}
	}
	else {
		if (dx >= -dy) {
			d = 2 * dy + dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy + dx);
			while (x < x2) {
				if (d > 0) {
					d += delta_e;
					x++;
				}
				else {
					d += delta_ne;
					points[y - min_y].first = x;
					points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
					x++;
					y--;
				}
			}
			points[y - min_y].first = x;
			points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
		}
		else {
			points[y - min_y].first = x;
			points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
			d = 2 * dx + dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx + dy);
			while (y > y2) {
				if (d < 0) {
					d += delta_e;
					y--;
				}
				else {
					d += delta_ne;
					x++;
					y--;
				}
				points[y - min_y].first = x;
				points[y - min_y].second = get_approx_z(POINT{ x, y }, v1, v2);
			}
		}
	}
}

COLORREF CCGWorkView::CRenderer::calculate_light(const vec3& point, const vec3& normal)
{
	COLORREF color = multiply(ambiant.color, ambiant.intensity);
	for (const LightParams& light : lights) {
		if (!light.enabled) {
			continue;
		}

		// Diffuse calculation
		vec3 N = normal;
		vec3 L;
		if (light.type == LIGHT_POINT) {
			L = normalized(light.data - point);
		}
		else {
			L = normalized(light.data);
		}
		float diffuse = light.diffuse * dot(L, N);
		color = add(color, multiply(light.color, diffuse));

		// Specular calculation
		vec3 V = normalized(-point);
		vec3 R = N * 2 * dot(L, N) - L;
		float specular = light.specular * std::pow(dot(R, V), ambiant.exponent);
		color = add(color, multiply(light.color, specular));
	}
	return color;
}

void CCGWorkView::CRenderer::apply_perspective(vec4 & v)
{
	float f = 1.0f / v.w;
	v = scaling(f, f, f) * v;
} 

vec3 CCGWorkView::CRenderer::cast(const vec3& v)
{
	int width = screen.Width(), height = screen.Height();
	int scale = min(height, width);
	vec3 v2(v.x * scale + width / 2, -v.y * scale + height / 2, v.z);
	return v2;
}

void CCGWorkView::CRenderer::set_bitmap_dimensions(const BITMAPINFO& info)
{
	BYTE* tmp = bitmap;
	bitmap = new BYTE[info.bmiHeader.biSizeImage];
	if (tmp != nullptr) {
		delete[] tmp;
	}
	parent->GetClientRect(&screen);
}

void CCGWorkView::CRenderer::get_bitmap(CDC * context)
{
	context->FillSolidRect(&screen, background_color);
	int i = GetDIBits(*context, parent->m_pDbBitMap, 0, parent->BMInfo.bmiHeader.biHeight,
		bitmap, &(parent->BMInfo), DIB_RGB_COLORS);
}

void CCGWorkView::CRenderer::draw_bitmap(CDC * context)
{
	int i = SetDIBits(*context, parent->m_pDbBitMap, 0, parent->BMInfo.bmiHeader.biHeight,
		bitmap, &(parent->BMInfo), DIB_RGB_COLORS);
}

void CCGWorkView::CRenderer::set_camera(const CCamera & camera)
{
	this->camera = camera;
}

void CCGWorkView::CRenderer::draw_normal(const vec3& origin, const vec3& direction, COLORREF color) {
	vec3 goal = (origin + normalized(direction) * NORMAL_SIZE_FACTOR);

	vec4 a = camera.projection * origin;
	vec4 b = camera.projection * goal;
	if (!camera.is_orthographic()) {
		apply_perspective(a);
		apply_perspective(b);
	}

	if (a.z >= 0 && b.z >= 0) {
		vec3 newSource = cast(vec3(a));
		newSource.z = origin.z;
		vec3 newStart = cast(vec3(b));
		newStart.z = goal.z;
		draw_line(newStart, newSource, color);
	}

}

bool pointIsInside(vector<vec3> points, vec2 dot) {

	unsigned int i, j = points.size() - 1;
	bool res = false;

	for (i = 0; i < points.size(); j = i, i++) {
		if (points[i].y < dot.y && points[j].y >= dot.y || points[j].y < dot.y && points[i].y >= dot.y) {
			if (points[i].x + (dot.y - points[i].y) / (points[j].y - points[i].y)*(points[j].x - points[i].x) < dot.x) {
				res = !res;
			}
		}
	}

	return res;
}

void CCGWorkView::CRenderer::draw_bounding_box(const CModel& model) {
	mat4 transform = model.model_transform * model.view_transform;
	for (const vec3& p1 : model.bounding_box) {
		for (const vec3& p2 : model.bounding_box) {
			vec3 res = p1 - p2;
			if ((!res.x && !res.y) || (!res.y && !res.z) || (!res.x && !res.z)) {
				vec4 a = transform * vec4(p1.x, p1.y, p1.z, 1.0f);
				vec4 b = transform * vec4(p2.x, p2.y, p2.z, 1.0f);
				float az = a.z, bz = b.z;
				a = camera.projection * a;
				b = camera.projection * b;
				if (!camera.is_orthographic()) {
					apply_perspective(a);
					apply_perspective(b);
				}
				vec3 aa = cast(vec3(a));
				aa.z = az;
				vec3 bb = cast(vec3(b));
				bb.z = bz;
				draw_line(aa, bb, bbox_color);
			}
		}
	}
}

void CCGWorkView::CRenderer::draw_model(const CModel & model)
{
	if (rendering_type == SQUELETON) {
		draw_edges(model);
	}
	else {
		switch (shading_type) {
		case FLAT:
			draw_flat(model);
			break;
		case GOURAUD:
			draw_gouraud(model);
			break;
		case PHONG:
			break;
		}
	}
	if (draw_polygon_normals || draw_vertice_normals) {
		draw_normals(model);
	}
	if (draw_bbox) {
		draw_bounding_box(model);
	}

	select_highlighted_pol = false;
}

float CCGWorkView::CRenderer::get_x(vec3 v1, vec3 v2, int y)
{
	float d = v2.y - v1.y;
	if (d < 1.0f && d > -1.0f) {
		return v2.x;
	}
	float t = ((float)y - v1.y) / d;
	if (t < 0.0f) {
		return v1.x;
	}
	if (t > 1.0f) {
		return v2.x;
	}
	return v1.x + t * (v2.x - v1.x);
}

void CCGWorkView::CRenderer::draw_flat(const CModel & model)
{
	for (const CPolygon& polygon : model.polygons) {
		vec3 camera_view(0, 0, 1.0f);
		bool inverted = false;
		if (dot(camera_view, polygon.calculated_normal) > 0) {
			inverted = true;
		}
		vector<vec3> points;
		int first = 0, last = 0, i = 0;
		int min_y = screen.Height(), max_y = 0;
		for (const CVertice& vertice : polygon.vertices) {
			vec4 projected = camera.projection * vertice.transformed;
			if (!camera.is_orthographic()) {
				apply_perspective(projected);
			}
			vec3 v = cast(vec3(projected));
			v.z = vertice.transformed.z;
			if (v.y < min_y) {
				first = i;
				min_y = v.y;
			}
			if (v.y > max_y) {
				last = i;
				max_y = v.y;
			}
			if (inverted) {
				points.insert(points.begin(), v);
			}
			else {
				points.push_back(v);
			}
			i++;
		}
		if (inverted) {
			first = i - first - 1;
			last = i - last - 1;
		}

		int l = first, r = first;
		int ll = l - 1;
		if (ll < 0) {
			ll += i;
		}
		int rr = r + 1;
		if (rr >= i) {
			rr -= i;
		}
		vector<pair<int, float>> left(max_y - min_y + 1), right(max_y - min_y + 1);
		while (true) {
			calculate_left(points[l], points[ll], left, min_y);
			if (ll == last) {
				break;
			}
			l--;
			ll--;
			if (l < 0) {
				l += i;
			}
			if (ll < 0) {
				ll += i;
			}
		}
		while (true) {
			calculate_right(points[r], points[rr], right, min_y);
			if (rr == last) {
				break;
			}
			r++;
			rr++;
			if (r >= i) {
				r -= i;
			}
			if (rr >= i) {
				rr -= i;
			}
		}
		COLORREF color = calculate_light(polygon.origin_transformed, polygon.calculated_normal);
		for (int y = min_y; y <= max_y; y++) {
			vec3 v1(left[y - min_y].first, y, left[y - min_y].second);
			vec3 v2(right[y - min_y].first, y, right[y - min_y].second);
			for (int x = v1.x; x <= v2.x; x++) {
				set_pixel(POINT{ x, y }, v1, v2, color);
			}
		}
	}
}

void CCGWorkView::CRenderer::draw_gouraud(const CModel & model)
{
	for (const CPolygon& polygon : model.polygons) {
		vec3 camera_view(0, 0, 1.0f);
		bool inverted = false;
		if (dot(camera_view, polygon.calculated_normal) > 0) {
			inverted = true;
		}

		bool draw = false;

		vector<vec3> points;
		vector<COLORREF> colors;
		int first = 0, last = 0, i = 0;
		int min_y = screen.Height(), max_y = 0;
		for (const CVertice& vertice : polygon.vertices) {
			vec4 projected = camera.projection * vertice.transformed;
			if (!camera.is_orthographic()) {
				apply_perspective(projected);
			}
			vec3 v = cast(vec3(projected));
			v.z = vertice.transformed.z;
			if (v.x >= 0 && v.x < screen.Width() && v.y >= 0 && v.y < screen.Height()) {
				draw = true;
			}
			if (v.y < min_y) {
				first = i;
				min_y = v.y;
			}
			if (v.y > max_y) {
				last = i;
				max_y = v.y;
			}
			if (inverted) {
				points.insert(points.begin(), v);
				colors.insert(colors.begin(), calculate_light(vertice.transformed, vertice.calculated_normal));
			}
			else {
				points.push_back(v);
				colors.push_back(calculate_light(vertice.transformed, vertice.calculated_normal));
			}
			i++;
		}
		if (!draw) {
			continue;
		}
		if (inverted) {
			first = i - first - 1;
			last = i - last - 1;
		}

		int l = first, r = first;
		int ll = l - 1;
		if (ll < 0) {
			ll += i;
		}
		int rr = r + 1;
		if (rr >= i) {
			rr -= i;
		}
		vector<pair<int, float>> left(max_y - min_y + 1), right(max_y - min_y + 1);
		vector<COLORREF> color_left(max_y - min_y + 1), color_right(max_y - min_y + 1);
		while (true) {
			calculate_left(points[l], points[ll], left, min_y);
			int start = (int)(points[l].y) - min_y, stop = (int)(points[ll].y) - min_y;
			COLORREF c1 = colors[l], c2 = colors[ll];
			for (int i = start; i <= stop; i++) {
				float t;
				if (start == stop) {
					if (points[r].x < points[rr].x) {
						t = 0.0f;
					}
					else {
						t = 1.0f;
					}
				}
				else if (std::abs(points[ll].x - points[l].x) > stop - start) {
					t = (float)(left[i].first - (int)(points[l].x)) / (points[ll].x - points[l].x);
				}
				else {
					t = (float)(i - start) / (float)(stop - start);
				}


				color_left[i] = add(multiply(c1, (1.0f - t)), multiply(c2, t));
			}

			if (ll == last) {
				break;
			}
			l--;
			ll--;
			if (l < 0) {
				l += i;
			}
			if (ll < 0) {
				ll += i;
			}
		}
		while (true) {
			calculate_right(points[r], points[rr], right, min_y);
			int start = (int)(points[r].y) - min_y, stop = (int)(points[rr].y) - min_y;
			for (int i = start; i <= stop; i++) {
				float t;
				if (start == stop) {
					if (points[r].x < points[rr].x) {
						t = 1.0f;
					}
					else {
						t = 0.0f;
					}
				}
				else if (std::abs(points[rr].x - points[r].x) > stop - start) {
					t = (float)(right[i].first - (int)(points[r].x)) / (points[rr].x - points[r].x);
				}
				else {
					t = (float)(i - start) / (float)(stop - start);
				}
				color_right[i] = add(multiply(colors[r], (1.0f - t)), multiply(colors[rr], t));
			}

			if (rr == last) {
				break;
			}
			r++;
			rr++;
			if (r >= i) {
				r -= i;
			}
			if (rr >= i) {
				rr -= i;
			}
		}
		for (int y = min_y; y <= max_y; y++) {
			vec3 v1(left[y - min_y].first, y, left[y - min_y].second);
			vec3 v2(right[y - min_y].first, y, right[y - min_y].second);
			for (int x = v1.x; x <= v2.x; x++) {
				float t;
				if (v2.x - v1.x < 1.0f && v2.x - v1.x > -1.0f) {
					t = 0.5f;
				}
				else {
					t = ((float)(x)-v1.x) / (v2.x - v1.x);
				}
				COLORREF color = add(multiply(color_left[y - min_y], (1.0f - t)), multiply(color_right[y - min_y], t));
				set_pixel(POINT{ x, y }, v1, v2, color);
			}
		}
	}
}

void CCGWorkView::CRenderer::draw_phong(const CModel & model)
{

}

void CCGWorkView::CRenderer::draw_edges(const CModel & model)
{
	vector<vector<edge>> edges_all;
	vector<bool> isItHidden;
	bool toDraw = true;
	for (const CPolygon& polygon : model.polygons) {
		toDraw = true;
		vec3 camera_view(0, 0, 1.0f);
		if (backface_culling && dot(camera_view, polygon.calculated_normal) > 0) {
			toDraw = false;
			//continue;
		}

		vector<vec3> points;
		vector<edge> edges;
		for (const CVertice& vertice : polygon.vertices) {
			vec4 projected = camera.projection * vertice.transformed;
			if (!camera.is_orthographic()) {
				apply_perspective(projected);
			}
			vec3 v = cast(vec3(projected));
			v.z = vertice.transformed.z;
			points.push_back(v);
		}
		int size = points.size() - 1;

		COLORREF polygon_color = wireframe_color;
		bool highlight = false;
		if (select_highlighted_pol && pointIsInside(points, vec2((float)mouse_x, (float)mouse_y))) {
			highlight = true;
			polygon_color = highlight_polygon;
		}


		for (int i = 0; i < size; i++) {
			if (toDraw)
				draw_line(points[i], points[i + 1], polygon_color, true);
			edges.push_back(edge(points[i], points[i + 1]));
		}
		if (toDraw)
			draw_line(points[size], points[0], polygon_color, true);
		edges.push_back(edge(points[size], points[0]));

		edges_all.push_back(edges);
		isItHidden.push_back(dot(camera_view, polygon.calculated_normal) > 0);
	}

	for (int i = 0; i < edges_all.size(); i++) {	//pour chaque polygone
		for (int a = 0; a < edges_all[i].size(); a++) {	//pour chaque edge du polygon
			for (int j = 0; j < edges_all.size(); j++) {	//check si dans un des polygones
				if (i == j) continue;
				for (int b = 0; b < edges_all[j].size(); b++) {	//il existe un edge egal
					if (edges_all[i][a] == edges_all[j][b]) {
						if (isItHidden[i] != isItHidden[j]) {	//et que les 2 polygones soient shown + hidden
							draw_line(edges_all[i][a].first, edges_all[i][a].second, RGB(255, 0, 140), true);
						}
					}
				}
			}
		}

	}
}

void CCGWorkView::CRenderer::draw_normals(const CModel & model)
{
	mat4 transform = model.model_transform * model.view_transform;
	float inverted_polygon = 1.0f;
	float inverted_vertice = 1.0f;
	if (invert_polygon_normals) {
		inverted_polygon = -1.0f;
	}
	if (invert_vertice_normals) {
		inverted_vertice = -1.0f;
	}
	for (const CPolygon& polygon : model.polygons) {
		vec3 camera_view(0, 0, 1.0f);
		if (backface_culling && dot(camera_view, polygon.calculated_normal) > 0) {
			continue;
		}

		if (draw_polygon_normals) {
			if (draw_polygon_included_normals) {
				vec3 dest = polygon.origin + polygon.included_normal;
				dest = transform * dest;
				draw_normal(polygon.origin_transformed, (dest - polygon.origin) * inverted_polygon, normals_color);
			}
			else {
				draw_normal(polygon.origin_transformed, polygon.calculated_normal * inverted_polygon, normals_color);
			}
		}
		if (draw_vertice_normals) {
			for (const CVertice& vertice : polygon.vertices) {
				if (draw_vertice_included_normals) {
					vec3 dest = vertice.point + vertice.included_normal;
					dest = transform * dest;
					draw_normal(vertice.transformed, (dest - vertice.point) * inverted_vertice, normals_color);
				}
				else {
					draw_normal(vertice.transformed, vertice.calculated_normal * inverted_vertice, normals_color);
				}
			}
		}
	}
}


COLORREF CCGWorkView::CRenderer::multiply(COLORREF color, float k)
{
	int R = max(0, min(255, (float)(GetRValue(color)) * k));
	int G = max(0, min(255, (float)(GetGValue(color)) * k));
	int B = max(0, min(255, (float)(GetBValue(color)) * k));
	return RGB(R, G, B);
}

COLORREF CCGWorkView::CRenderer::add(COLORREF c1, COLORREF c2)
{
	int R = min(255, GetRValue(c1) + GetRValue(c2));
	int G = min(255, GetGValue(c1) + GetGValue(c2));
	int B = min(255, GetBValue(c1) + GetBValue(c2));
	return RGB(R, G, B);
	}