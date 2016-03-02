#pragma once
#ifndef VEC4_H
#define VEC4_H
#include <iostream>
#include <cmath>
#include "Vec3.h"

using namespace std;


class Vec4 {
public:
	float x, y, z, w = 0;

	Vec4() = default;
	Vec4(float a, float b, float c) : x(a), y(b), z(c) { }
	Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) { }

	Vec4(const Vec4 &orig)
	{
		x = orig.x;
		y = orig.y;
		z = orig.z;
		w = orig.w;
	}

	Vec4 operator+(const Vec4 &) const;
	Vec4 operator-(const Vec4 &) const;
	Vec4 operator*(const float &) const;    //浮点数点乘
	Vec4 operator*(const int &) const;       //整数点乘
	float operator%(const Vec4 &) const;    //数乘
	Vec4 operator^(const Vec4 &) const;      //叉乘
	float length() const;
	Vec4 norm();

	virtual ~Vec4() = default;


	static const Vec4 Zero;
	static const Vec4 XAxis;
	static const Vec4 YAxis;
	static const Vec4 ZAxis;
};


ostream &print(ostream &, const Vec4 &);


class Point4 : public Vec4 {
public:
	Point4() = default;
	Point4(float a, float b, float c) : Vec4(a, b, c) { Vec4::w = 1; }
	Point4(Point3 p) : Vec4(p.x, p.y, p.z, 1) { }
	Point4(float a, float b, float c, float d) : Vec4(a, b, c, d) { }
	Point4(const Point4 &);
	Vec4 operator-(const Point4 &) const;      //由两点位置得到向量
	Point4 operator-(const Vec4 &) const;	   //由点与向量，得到新的点
	Point4 operator+(const Vec4 &) const;
};
Vec4 faceNorm(const Point4 &, const Point4 &, const Point4 &);  //由平面三个顶点位置，求出平面法线向量

#endif


