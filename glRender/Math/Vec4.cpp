#include "Vec4.h"

const Vec4 Vec4::Zero(0, 0, 0, 0);
const Vec4 Vec4::XAxis(1, 0, 0, 0);
const Vec4 Vec4::YAxis(0, 1, 0, 0);
const Vec4 Vec4::ZAxis(0, 0, 1, 0);

Vec4
Vec4::operator+(const Vec4 &rv) const {
	return Vec4(x + rv.x, y + rv.y, z + rv.z);
}

Vec4
Vec4::operator-(const Vec4 &rv) const {
	return Vec4(x - rv.x, y - rv.y, z - rv.z);
}

Vec4
Vec4::operator*(const float &m) const {          //���������
	return Vec4(x * m, y * m, z * m);
}

Vec4 
Vec4::operator*(const int &m) const {              //�������
	return Vec4(x * m, y * m, z * m);
}

float
Vec4::operator%(const Vec4 &rv) const {          //����
	return (x * rv.x + y * rv.y + z * rv.z);
}

Vec4
Vec4::operator^(const Vec4 &rv) const {          //���
	return Vec4(y * rv.z - z * rv.y, z * rv.x - x * rv.z, x * rv.y - y * rv.x);
}

float
Vec4::length() const {
	return sqrt(x * x + y * y + z * z);
}

Vec4
Vec4::norm() {                            //��λ����
	float len = this->length();
	//return (*this * (1 / len));

	if (len > 0.0)
	{
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}
	return *this;
}

Point4::Point4(const Point4 &orig) :
	Vec4(orig) { }

//������λ�õõ�����
Vec4 
Point4::operator-(const Point4 &r) const {
	Vec4 result = { this->x - r.x, this->y - r.y, this->z - r.z };
	return result;
}

Point4 Point4::operator-(const Vec4 &r) const
{
	Point4 result = { this->x - r.x, this->y - r.y, this->z - r.z };
	return result;
}

Point4 Point4::operator+(const Vec4 &r) const
{
	Point4 result = { this->x + r.x, this->y + r.y, this->z + r.z };
	return result;
}


//�ǳ�Ա����
ostream &print(ostream &os, const Vec4 &a) {
	os << a.x << " " << a.y << " " << a.z << " " << a.w << endl;
	return os;
}

Vec4 faceNorm(const Point4 &a, const Point4 &b, const Point4 &c) { //��ƽ����������λ�ã����ƽ�淨������
	float A, B, C;
	A = a.y * (b.z - c.z) + b.y * (c.z - a.z) + c.y * (a.z - b.z);
	B = a.z * (b.x - c.x) + b.z * (c.x - a.x) + c.z * (a.x - b.x);
	C = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
	Vec4 r = { A, B, C };
	return r;
}
