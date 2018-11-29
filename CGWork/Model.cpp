#include "Model.h"

CModel::CModel(COLORREF color, COLORREF normalsColor, COLORREF bbox_color) :
	color(color), normalsColor(normalsColor), bbox_color(bbox_color) {}

void CModel::setup_model()
{
	vector<vec3> bounds(2);
	bool first = true;

	// Create bounding box
	for (const CPolygon& polygon : polygons) {
		for (const CVertice& vertice : polygon.vertices) {
			vec3 point = vertice.point;
			if (first) {
				bounds[0] = point;
				bounds[1] = point;
				first = false;
			}
			else {
				bounds[0].x = min(bounds[0].x, point.x);
				bounds[0].y = min(bounds[0].y, point.y);
				bounds[0].z = min(bounds[0].z, point.z);
				bounds[1].x = max(bounds[1].x, point.x);
				bounds[1].y = max(bounds[1].y, point.y);
				bounds[1].z = max(bounds[1].z, point.z);
			}
		}
	}
	bounding_box.clear();
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				bounding_box.push_back(vec3(bounds[i].x, bounds[j].y, bounds[k].z));
			}
		}
	}

	// Transform object to be in center of the screen
	vec3 origin = (bounds[0] + bounds[1]) / 2;
	float scale = 0.9f * (1.0f / max(max(bounds[1].x - bounds[0].x, bounds[1].y - bounds[0].y), bounds[1].z - bounds[0].z));
	mat4 transform = scaling(scale, scale, scale);
	transform = translation(-origin.x, -origin.y, -origin.z) * transform;
	for (CPolygon& polygon : polygons) {
		for ( CVertice& vertice : polygon.vertices) {
			vertice.point = transform * vec4(vertice.point.x, vertice.point.y, vertice.point.z, 1.0f);
		}
	}
	for (vec3& point : bounding_box) {
		point = transform * vec4(point.x, point.y, point.z, 1.0f);
	}

}

void CModel::add_polygon(const CPolygon & polygon)
{
	polygons.push_back(polygon);
}

void CModel::transform_model(const mat4 & m)
{
	model_transform = m * model_transform;
}

void CModel::transform_view(const mat4 & m)
{
	view_transform = m * view_transform;
}
