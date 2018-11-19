#pragma once
#include <vector>
#include "CObject.h"
#include "Mat.h"

using namespace std;

class Model
{
public:
	vector<ModelObject> objects;
	Mat transform;
	Mat position;

	Model();
	~Model();
	void reset();
	void add_object(const ModelObject& object);
};

