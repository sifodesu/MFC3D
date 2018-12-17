#include "stdafx.h"

#include "CGWorkView.h"

CCGWorkView::CRenderer::CRenderer(CCGWorkView* parent) :
	parent(parent), bitmap(nullptr),
	background_color(BLACK),
	highlight_polygon(GREEN),
	normals_color(PINK),
	wireframe_color(WHITE)
{
	draw_bbox = false;
	draw_polygon_normals = false;
	draw_vertice_normals = false;
	draw_polygon_included_normals = true;
	draw_vertice_included_normals = true;
	select_highlighted_pol = false;

	backface_culling = true;
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
	if (offset > parent->BMInfo.bmiHeader.biSizeImage || offset < 0 || p.x >= screen.Width() || p.x < 0) {
		return;
	}
	bitmap[offset] = GetBValue(c);
	bitmap[offset + 1] = GetGValue(c);
	bitmap[offset + 2] = GetRValue(c);
}

//approximate the z of a point on a line
float get_approx_z(POINT p, const vec3& source, const vec3& dest) {
	float normS = norm(vec2(vec3((float)p.x, (float)p.y, 0) - source));
	float normD = norm(vec2(dest - vec3((float)p.x, (float)p.y, 0)));
	return source.z*normS / (normS + normD) + dest.z*normD / (normS + normD);
}

//approximate the z of a point inside a polygon
float approximate_z_in_pol(vector<vec3>& points, vec2& p) {
	float sum = 0.0f;

	for (vec3 point : points) {
		sum += norm(p - vec2(point));
	}
	float res = 0.0f;
	for (vec3 point : points) {
		res += point.z*norm(p - vec2(point)) / sum;
	}

	return res;
}

void CCGWorkView::CRenderer::set_pixel(POINT p, const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint) {
	if (p.x >= 0 && p.y >= 0 && p.x < min(3840, screen.Width()) && p.y < min(2160, screen.Height())) {
		if (!bitFlag.test(p.x + p.y * 3840)) {
			bitFlag.set(p.x + p.y * 3840);
			draw_pixel(p, color);
			z_buffer[p.y][p.x] = get_approx_z(p, v1, v2);
		}
		else {
			if (z_buffer[p.y][p.x] > get_approx_z(p, v1, v2)) {
				draw_pixel(p, color);
				z_buffer[p.y][p.x] = get_approx_z(p, v1, v2);
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
	if (bitmap != nullptr) {
		delete[] bitmap;
	}
	bitmap = new BYTE[info.bmiHeader.biSizeImage];
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
		newSource.z = a.z;
		vec3 newStart = cast(vec3(b));  
		newStart.z = b.z;
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
	mat4 transform = model.model_transform * model.view_transform * camera.projection;
	for (const vec3& p1 : model.bounding_box) {
		for (const vec3& p2 : model.bounding_box) {
			vec3 res = p1 - p2;
			if ((!res.x && !res.y) || (!res.y && !res.z) || (!res.x && !res.z)) {
				vec4 a = transform * vec4(p1.x, p1.y, p1.z, 1.0f);
				vec4 b = transform * vec4(p2.x, p2.y, p2.z, 1.0f);
				if (!camera.is_orthographic()) {
					apply_perspective(a);
					apply_perspective(b);
				}
				vec3 p1 = cast(vec3(a));
				vec3 p2 = cast(vec3(b));
				draw_line(p1, p2, model.bbox_color);
			}
		}
	}
}

void CCGWorkView::CRenderer::draw_model(const CModel & model)
{
	draw_edges(model);
	if (draw_polygon_normals || draw_vertice_normals) {
		draw_normals(model);
	}
	if (draw_bbox) {
		draw_bounding_box(model);
	}

	select_highlighted_pol = false;
}

void CCGWorkView::CRenderer::draw_edges(const CModel & model)
{
	for (const CPolygon& polygon : model.polygons) {
		vec3 camera_view(0, 0, 1.0f);
		if (dot(camera_view, polygon.calculated_normal) > 0) {
			continue;
		}

		vector<vec3> points;
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
			draw_line(points[i], points[i + 1], polygon_color, highlight);
		}
		draw_line(points[size], points[0], polygon_color, highlight);
	}
}

void CCGWorkView::CRenderer::draw_normals(const CModel & model)
{
	mat4 transform = model.model_transform * model.view_transform;
	for (const CPolygon& polygon : model.polygons) {
		vec3 camera_view(0, 0, 1.0f);
		if (dot(camera_view, polygon.calculated_normal) > 0) {
			continue;
		}

		if (draw_polygon_normals) {
			if (draw_polygon_included_normals) {
				vec3 dest = polygon.origin + polygon.included_normal;
				dest = transform * dest;
				draw_normal(polygon.origin_transformed, dest - polygon.origin, normals_color);
			}
			else {
				draw_normal(polygon.origin_transformed, polygon.calculated_normal, normals_color);
			}
		}
		if (draw_vertice_normals) {
			for (const CVertice& vertice : polygon.vertices) {
				if (draw_vertice_included_normals) {
					vec3 dest = vertice.point + vertice.included_normal;
					dest = transform * dest;
					draw_normal(vertice.transformed, dest - vertice.point, normals_color);
				}
				else {
					draw_normal(vertice.transformed, vertice.calculated_normal, normals_color);
				}
			}
		}
	}
}
