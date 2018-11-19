#include "Mat.h"

Mat::Mat(float s)
{
	m[0].x = s;
	m[1].y = s;
	m[2].z = s;
	m[3].w = s;
}


Mat::Mat(const Vec& v1, const Vec& v2, const Vec& v3, const Vec& v4)
{
	m[0] = v1;
	m[1] = v2;
	m[2] = v3;
	m[3] = v4;
}

Mat::~Mat()
{
}

const Vec & Mat::operator[](int i) const
{
	return m[i];
}

Vec & Mat::operator[](int i)
{
	return m[i];
}

Mat Mat::operator+(const Mat & m) const
{
	return Mat(
		this->m[0] + m[0],
		this->m[1] + m[1],
		this->m[2] + m[2],
		this->m[3] + m[3]
	);
}

Mat Mat::operator*(float s) const
{
	return Mat(
		m[0] * s,
		m[1] * s,
		m[2] * s,
		m[3] * s
	);
}

Mat Mat::operator*(const Mat & m) const
{
	Mat new_mat(0.0f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				new_mat[i][k] += this->m[i][j] * m[j][k];
			}
		}
	}
	return new_mat;
}

Vec Mat::operator*(const Vec & v) const
{
	Vec new_vec;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			new_vec[i] += this->m[i][j] * v[j];
		}
	}
	return new_vec;
}
