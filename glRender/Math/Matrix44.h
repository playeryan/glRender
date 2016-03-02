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
	Point4 operator*(ScreenPoint3);             //���ڴ���Ļ��������õ����صĹ۲�����ϵ������
	Vec4 operator*(const Vec4);
	Point4 operator*(const Point4);             //�����������ѹ�һ�ĵ���о���˷�
	Point4 operator^(const Point4);             //����������δ��һ�ĵ���о���˷�

	//��������صĳ�Ա����
	Matrix44& Frustum(float, float, float, float, float, float);   //���ù۲�ƽ��ͷ��
	Matrix44& Ortho(float, float, float, float, float, float);    //������ͶӰ����
	Matrix44& viewport(float, float, float, float);               //���ù淶�����嵽��Ļ�ӿڵı任����
	float GetSubDeterminant(const int&, const int&);
	float GetDeterminant();     //������ʽ
	Matrix44 GetTranspose();     //��ת�þ���
	Matrix44 Inverse();          //�������
	Matrix44& LoadIdentity();
	Matrix44& Translatef(float, float, float);    //��άƽ��
	Matrix44& Rotatef(float, float, float, float);   //��ά��ת(ʹ����Ԫ��)�������ֱ�Ϊ�Ƕȡ�������
	Matrix44& Scalef(float, float, float);       //��ά����
	Matrix44& LookAt(float, float, float, float, float, float, float, float, float);   //���ù۲�λ�á�����
	Matrix44& LookAt(Vec4 eyePosition, Vec4 nVector, Vec4 upVector);
	Matrix44& Perspective(float, float, float, float);  ////ͨ���ӳ��ǵȲ������ù۲�ƽ��ͷ��
	Matrix44& SetElement(int, int, float);
};

void Mprint(const Matrix44&);


#endif
