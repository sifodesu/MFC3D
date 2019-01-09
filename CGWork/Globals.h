#pragma once
#include "StdAfx.h"

const float PI = 3.1415926535f;

const float TRANSLATION_FACTOR = 0.0015f;
const float ROTATION_FACTOR = 0.2f;
const float SCALING_FACTOR = 0.002f;

const float DEPTH_FACTOR = 5.0f;
const float NORMAL_SIZE_FACTOR = 0.06f;

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PINK = RGB(255, 0, 255);
const COLORREF CYAN = RGB(0, 255, 255);

typedef enum {
	BOX = 0,
	TRIANGLE = 1,
	GAUSSIAN = 2,
	SINC = 3
} AAFilter;

typedef enum {
	X3 = 0,
	X5 = 1,
} AAKernel;

const float RATE[4][2] = {
	{ 1.0f / 9.0f, 1.0f / 25.0f },  // Box
	{ 1.0f / 16.0f, 1.0f / 81.0f }, // Triangle
	{ 1.0f / 17.0f, 1.0f / 50.0f }, // Gaussian
	{ 1.0f / 24.0f, 1.0f / 33.0f }, // Sinc
};

const float WEIGHTS_3X3[4][3][3] = {
	{ // Box
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}
	},
	{ // Triangle
		{1.0f, 2.0f, 1.0f},
		{2.0f, 4.0f, 2.0f},
		{1.0f, 2.0f, 1.0f}
	},
	{ // Gaussian
		{1.0f, 2.0f, 1.0f},
		{2.0f, 5.0f, 2.0f},
		{1.0f, 2.0f, 1.0f}
	},
	{ // Sinc
		{2.0f, 3.0f, 2.0f},
		{3.0f, 4.0f, 3.0f},
		{2.0f, 3.0f, 2.0f}
	}
};

const float WEIGHTS_5X5[4][5][5] = {
	{ // Box
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}
	},
	{ // Triangle
		{1.0f, 2.0f, 3.0f, 2.0f, 1.0f},
		{2.0f, 4.0f, 6.0f, 4.0f, 2.0f},
		{3.0f, 6.0f, 9.0f, 6.0f, 3.0f},
		{2.0f, 4.0f, 6.0f, 4.0f, 2.0f},
		{1.0f, 2.0f, 3.0f, 2.0f, 1.0f}
	},
	{ // Gaussian
		{1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
		{1.0f, 2.0f, 4.0f,  2.0f, 1.0f},
		{1.0f, 4.0f, 10.0f, 4.0f, 1.0f},
		{1.0f, 2.0f, 4.0f,  2.0f, 1.0f},
		{1.0f, 1.0f, 1.0f,  1.0f, 1.0f}
	},
	{ // Sinc
		{-2.0f, -1.0f, 0.0f, -1.0f, -2.0f},
		{-1.0f,  4.0f, 6.0f,  4.0f, -1.0f},
		{ 0.0f,  6.0f, 9.0f,  6.0f,  0.0f},
		{-1.0f,  4.0f, 6.0f,  4.0f, -1.0f},
		{-2.0f, -1.0f, 0.0f, -1.0f, -2.0f}
	}
};