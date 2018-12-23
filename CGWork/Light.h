#pragma once
#include "Vec.h"
#include "Globals.h"

const int LIGHT_NUM = 7;

typedef  enum 
{
    LIGHT_POINT,
    LIGHT_PARALLEL,
} LightType;

class LightParams
{
public:
    bool enabled;
    LightType type;
	COLORREF color;
	vec3 data;
	float diffuse;
	float specular;
    
    LightParams(): enabled(false) ,type(LIGHT_POINT), color(WHITE), diffuse(0.0f), specular(0.0f) {}
};

class AmbiantLightParams
{
public:
	COLORREF color;
	float intensity;
	float exponent;

	AmbiantLightParams() : color(WHITE), intensity(0.0f), exponent(0.0f) {}
};
