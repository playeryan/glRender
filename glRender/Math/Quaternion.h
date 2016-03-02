#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H
#include <cmath>
#include "Vec4.h"
#include "MathMacros.h"

//四元数数据结构
class Quat {
public:
	float x, y, z, w;                     //x,y，z对应于四元数的虚数部分，w对应实数部分

	Quat() = default;
	Quat(float a, float b, float c) : x(a), y(b), z(c), w(0) { }
	Quat(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) { }
	Quat(float dg, Vec4 axis) : x(sin(dg / 360.0 * PI) * axis.x),    //用旋转角、旋转轴构造而成的四元数
		                        y(sin(dg / 360.0 * PI) * axis.y),
		                        z(sin(dg / 360.0 * PI) * axis.z),
		                        w(cos(dg / 360.0 * PI)) { }
	//Quat& operator=(const Quat&);
	Quat(const Quat &);

	Quat operator+(const Quat &) const;   //四元数加法
	Quat operator-(const Quat &) const;   //四元数减法
	Quat operator*(const Quat &) const;   //四元数乘法
	float length() const;                //四元数向量的长度
	Quat norm();                          //单位向量（仅对虚数向量部分归一化）
	Quat conjugate();                     //共轭四元数
	Quat inverse();                       //四元数的逆；若四元数已归一化，则其逆等于其共轭四元数，无需此函数计算
	Vec4 rotate(Vec4);                    //旋转任意向量
	Point4 rotate(Point4);                //旋转任意点

	static const Quat Zero;
	static const Quat XAxis;
	static const Quat YAxis;
	static const Quat ZAxis;
	static const Quat UnitOne;
};

#endif
