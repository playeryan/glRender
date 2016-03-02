#pragma once
#ifndef VEC3_H
#define VEC3_H
#include <iostream>
#include <cmath>

using namespace std;

class Vec3 {
public:
	float x, y, z;

	Vec3() = default;
	Vec3(float a, float b, float c) : x(a), y(b), z(c) { }
	Vec3(float a, float b) : x(a), y(b), z(0.0) { }
	Vec3(float a) : x(a), y(0.0), z(0.0) { }
	//Vec3& operator=(const Vec3 &);

	Vec3(const Vec3 &);

	Vec3 operator+(const Vec3 &) const;
	Vec3 operator-(const Vec3 &) const;
	Vec3 operator*(const float &) const;
	float operator%(const Vec3 &) const;
	Vec3 operator^(const Vec3 &) const;
	float length() const;
	Vec3 norm() const;


	static const Vec3 Zero;
	static const Vec3 XAxis;
	static const Vec3 YAxis;
	static const Vec3 ZAxis;
};

//ostream &print(ostream &, const Vec3 &);



class Point3 : public Vec3 {
public:
	Point3() = default;
	Point3(float a, float b, float c) : Vec3(a, b, c) { }
	Point3(float a, float b) : Vec3(a, b) { }
	Point3(float a) : Vec3(a) { }
	Point3(const Point3 &);
};

class ScreenPoint3{   //成员分别为需绘制像素点的屏幕x、y坐标，和像素点对应观察坐标系下的真实z值倒数：1/z，用于深度比较
public:
	int x, y;
	float z;
	ScreenPoint3() = default;
	ScreenPoint3(int a, int b, float c) : x(a), y(b), z(c) {}
	//Point2(const Point2 &);
	ScreenPoint3& operator=(const ScreenPoint3&);
};

struct TexturePoint2 
{
	float x, y;
	TexturePoint2() = default;
	TexturePoint2(float a, float b) : x(a), y(b) {}
	TexturePoint2& operator=(const TexturePoint2&);
};

#endif

