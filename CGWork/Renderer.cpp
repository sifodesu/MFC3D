#include "stdafx.h"

#include "CGWorkView.h"

CCGWorkView::CRenderer::CRenderer(CCGWorkView* parent) :
	parent(parent), bitmap(nullptr),
	background_color(BLACK),
	highlight_polygon(GREEN),
	normals_color(PINK),
	wireframe_color(WHITE)
{
	draw_bounding_box = false;
	draw_polygon_normals = false;
	draw_vertice_normals = false;
	draw_polygon_included_normals = true;
	draw_vertice_included_normals = true;
	select_highlighted_pol = false;
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
	if (offset > parent->BMInfo.bmiHeader.biSizeImage || offset < 0 ||
		p.x >= screen.Width() || p.x < 0) {
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

void CCGWorkView::CRenderer::draw_normal(const vec3& startPoint, const vec3& givenNormal, mat4 transform, COLORREF color) {
	vec3 sourceNormal = startPoint + normalized(givenNormal)*0.02f;

	vec4 a = transform * vec4(sourceNormal.x, sourceNormal.y, sourceNormal.z, 1.0f);
	vec4 b = transform * vec4(startPoint.x, startPoint.y, startPoint.z, 1.0f);

	if (!camera.is_orthographic()) {
		apply_perspective(a);
		apply_perspective(b);
	}

	if (a.z >= 0 && b.z >= 0) {
		vec3 newSource = cast(vec3(a));
		vec3 newStart = cast(vec3(b));

		draw_line(newStart, newSource, color);
	}

}

bool CCGWorkView::CRenderer::check_if_drawn(const vec2& startPoint, const vec2& endPoint, std::unordered_set<edge>& current_set) {
	return false;
	edge tempEdge(startPoint, endPoint);
	if (current_set.find(tempEdge) == current_set.end()) {
		current_set.insert(tempEdge);
		return false;
	}
	return true;
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

void CCGWorkView::CRenderer::draw_bounding_box_if_needed(CModel& model, mat4& transform) {
	if (draw_bounding_box) {
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
}

void CCGWorkView::CRenderer::flood_fill(vector<vec3>& poly, vec2& p, COLORREF color) {
	if (pointIsInside(poly, p)) {
		//set_pixel(POINT {p.x, p.y}, )
	}
}

void CCGWorkView::CRenderer::draw_normals(CModel& model, CPolygon& polygon,
	mat4& transform, vector<vec3> source, vector<vec3> points, std::unordered_map<vec3, std::unordered_set<vec3>>& verticesMap) {
	vec3 sourceNormal;

	if (draw_polygon_included_normals) {
		sourceNormal = polygon.included_normal;
	}
	else if (source.size() >= 3) {
		sourceNormal = normalized(cross(source[2] - source[1], source[0] - source[1]));
	}
	if (draw_polygon_normals) {
		vec3 normalStart;
		for (const vec3& point : source) {
			normalStart = point + normalStart;
		}
		normalStart = normalStart / (float)source.size();

		draw_normal(normalStart, sourceNormal, transform, normals_color);
	}

	if (draw_vertice_normals && !draw_vertice_included_normals) {
		for (const vec3& point : source) {
			verticesMap[point].insert(sourceNormal);
		}
	}

	if (draw_vertice_normals && draw_vertice_included_normals) {
		for (const CVertice& point : polygon.vertices) {
			draw_normal(point.point, point.imported_normal, transform, normals_color);
		}
	}
}

void CCGWorkView::CRenderer::draw_model(CModel & model)
{
	mat4 transform = model.model_transform;
	transform = transform * model.view_transform;
	transform = transform * camera.projection;

	std::unordered_map<vec3, std::unordered_set<vec3>> verticesMap;
	std::unordered_set<edge> edgesDone;


	for (CPolygon& polygon : model.polygons) {
		vector<vec3> points;
		vector<vec3> source;
		for (const CVertice& vertice : polygon.vertices) {
			vec3 point = vertice.point;
			vec4 res = transform * vec4(point.x, point.y, point.z, 1.0f);
			if (!camera.is_orthographic()) {
				apply_perspective(res);
			}

			vec3 v = cast(vec3(res));
			points.push_back(v);
			source.push_back(point);
		}
		int size = points.size() - 1;

		if (select_highlighted_pol) {
			if (pointIsInside(points, vec2((float)mouse_x, (float)mouse_y))) {
				polygon.highlight = true;
			}
			else {
				polygon.highlight = false;
			}
		}

		for (int i = 0; i < size; i++) {
			draw_line(points[i], points[i + 1], polygon.highlight ? highlight_polygon : wireframe_color, polygon.highlight);
		}
		draw_line(points[size], points[0], polygon.highlight ? highlight_polygon : wireframe_color, polygon.highlight);

		draw_normals(model, polygon, transform, source, points, verticesMap);
	}
	if (draw_vertice_normals && !draw_vertice_included_normals) {
		for (auto i = verticesMap.begin(); i != verticesMap.end(); i++) {
			vec3 normalStart;
			for (const vec3& point : i->second) {
				normalStart = point + normalStart;
			}
			normalStart = normalStart / (float)i->second.size();

			draw_normal(i->first, normalStart, transform, normals_color);
		}
	}

	draw_bounding_box_if_needed(model, transform);

	select_highlighted_pol = false;
}
