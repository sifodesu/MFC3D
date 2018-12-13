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
	calculate_normals();
}

void CModel::calculate_normals()
{
	std::unordered_map<vec3, std::unordered_set<vec3>> vertice_map;
	for (CPolygon& polygon : polygons) {
		if (polygon.vertices.size() >= 3) {
			vec3 p1 = polygon.vertices[0].transformed;
			vec3 p2 = polygon.vertices[1].transformed;
			vec3 p3 = polygon.vertices[2].transformed;
			polygon.calculated_normal = normalized(cross(p3 - p2, p1 - p2));
			for (const CVertice& vertice : polygon.vertices) {
				vertice_map[vertice.point].insert(polygon.calculated_normal);
			}
		}
	}
	for (CPolygon& polygon : polygons) {
		for (CVertice& vertice : polygon.vertices) {
			vec3 calculated_normal;
			for (const vec3& normal : vertice_map[vertice.point]) {
				calculated_normal = calculated_normal + normal;
			}
			vertice.calculated_normal = calculated_normal / (float)vertice_map[vertice.point].size();
		}
	}
}

void CModel::add_polygon(const CPolygon & polygon)
{
	polygons.push_back(polygon);
}

void CModel::apply_transform()
{
	mat4 transform = model_transform * view_transform;
	for (CPolygon& polygon : polygons) {
		for (CVertice& vertice : polygon.vertices) {
			vertice.transformed = transform * vertice.point;
		}
		polygon.set_origin();
	}
	calculate_normals();
}

void CModel::transform_model(const mat4 & m)
{
	model_transform = m * model_transform;
	apply_transform();
}

void CModel::transform_view(const mat4 & m)
{
	view_transform = m * view_transform;
	apply_transform();
}
