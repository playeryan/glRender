#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H
#include <cmath>
#include "Vec4.h"
#include "MathMacros.h"

//��Ԫ�����ݽṹ
class Quat {
public:
	float x, y, z, w;                     //x,y��z��Ӧ����Ԫ�����������֣�w��Ӧʵ������

	Quat() = default;
	Quat(float a, float b, float c) : x(a), y(b), z(c), w(0) { }
	Quat(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) { }
	Quat(float dg, Vec4 axis) : x(sin(dg / 360.0 * PI) * axis.x),    //����ת�ǡ���ת�ṹ����ɵ���Ԫ��
		                        y(sin(dg / 360.0 * PI) * axis.y),
		                        z(sin(dg / 360.0 * PI) * axis.z),
		                        w(cos(dg / 360.0 * PI)) { }
	//Quat& operator=(const Quat&);
	Quat(const Quat &);

	Quat operator+(const Quat &) const;   //��Ԫ���ӷ�
	Quat operator-(const Quat &) const;   //��Ԫ������
	Quat operator*(const Quat &) const;   //��Ԫ���˷�
	float length() const;                //��Ԫ�������ĳ���
	Quat norm();                          //��λ���������������������ֹ�һ����
	Quat conjugate();                     //������Ԫ��
	Quat inverse();                       //��Ԫ�����棻����Ԫ���ѹ�һ��������������乲����Ԫ��������˺�������
	Vec4 rotate(Vec4);                    //��ת��������
	Point4 rotate(Point4);                //��ת�����

	static const Quat Zero;
	static const Quat XAxis;
	static const Quat YAxis;
	static const Quat ZAxis;
	static const Quat UnitOne;
};

#endif
