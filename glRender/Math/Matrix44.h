#pragma once
#ifndef MATRIX44_H
#define MATRIX44_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include "MathMacros.h"	
#include "Vec4.h"
#include "Vec3.h"

using namespace std;

#define STACK_SIZE 32

class Matrix44 {
public:
	float m[16];

public:
	Matrix44() = default;
	Matrix44(const Matrix44 &);
	Matrix44(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
	Matrix44(float[16]);

	Matrix44& operator=(const Matrix44&);
	Matrix44& operator+=(const Matrix44&);
	Matrix44& operator-=(const Matrix44&);
	Matrix44& operator*=(const Matrix44&);
	Matrix44& operator*=(float);

	Matrix44 operator+(const Matrix44&);
	Matrix44 operator-(const Matrix44&);
	Matrix44 operator*(const Matrix44&);
	Matrix44 operator*(float);
	Point4 operator*(ScreenPoint3);             //用于从屏幕像素坐标得到像素的观察坐标系下坐标
	Vec4 operator*(const Vec4);
	Point4 operator*(const Point4);             //用于齐坐标已归一的点进行矩阵乘法
	Point4 operator^(const Point4);             //用于齐坐标未归一的点进行矩阵乘法

	//功能性相关的成员函数
	Matrix44& Frustum(float, float, float, float, float, float);   //设置观察平截头体
	Matrix44& Ortho(float, float, float, float, float, float);    //设置正投影矩阵
	Matrix44& viewport(float, float, float, float);               //设置规范立方体到屏幕视口的变换矩阵
	float GetSubDeterminant(const int&, const int&);
	float GetDeterminant();     //求行列式
	Matrix44 GetTranspose();     //求转置矩阵
	Matrix44 Inverse();          //求逆矩阵
	Matrix44& LoadIdentity();
	Matrix44& Translatef(float, float, float);    //三维平移
	Matrix44& Rotatef(float, float, float, float);   //三维旋转(使用四元数)，参数分别为角度、轴向量
	Matrix44& Scalef(float, float, float);       //三维缩放
	Matrix44& LookAt(float, float, float, float, float, float, float, float, float);   //设置观察位置、方向
	Matrix44& LookAt(Vec4 eyePosition, Vec4 nVector, Vec4 upVector);
	Matrix44& Perspective(float, float, float, float);  ////通过视场角等参数设置观察平截头体
	Matrix44& SetElement(int, int, float);
};

void Mprint(const Matrix44&);


#endif
