#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::reset()
{
	vector<ModelObject> new_vector;
	Mat new_transform;
	objects = new_vector;
	transform = new_transform;
	position = new_transform;
}

void Model::add_object(const ModelObject& object)
{
	objects.push_back(object);
}
