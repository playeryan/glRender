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
	Vec4 operator*(const float &) const;    //���������
	Vec4 operator*(const int &) const;       //�������
	float operator%(const Vec4 &) const;    //����
	Vec4 operator^(const Vec4 &) const;      //���
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
	Vec4 operator-(const Point4 &) const;      //������λ�õõ�����
	Point4 operator-(const Vec4 &) const;	   //�ɵ����������õ��µĵ�
	Point4 operator+(const Vec4 &) const;
};
Vec4 faceNorm(const Point4 &, const Point4 &, const Point4 &);  //��ƽ����������λ�ã����ƽ�淨������

#endif


