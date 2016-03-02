#include "Vec3.h"

const Vec3 Vec3::Zero(0, 0, 0);
const Vec3 Vec3::XAxis(1, 0, 0);
const Vec3 Vec3::YAxis(0, 1, 0);
const Vec3 Vec3::ZAxis(0, 1, 1);


Vec3::Vec3(const Vec3 &orig) :
	x(orig.x), y(orig.y), z(orig.z) { }


Vec3
Vec3::operator+(const Vec3 &rv) const {
	return Vec3(x + rv.x, y + rv.y, z + rv.z);
}

Vec3
Vec3::operator-(const Vec3 &rv) const {
	return Vec3(x - rv.x, y - rv.y, z - rv.z);
}

Vec3
Vec3::operator*(const float &m) const {          //数乘
	return Vec3(x * m, y * m, z * m);
}

float
Vec3::operator%(const Vec3 &rv) const {          //点乘
	return x * rv.x + y * rv.y + z * rv.z;
}

Vec3
Vec3::operator^(const Vec3 &rv) const {          //叉乘
	return Vec3(y * rv.z - z * rv.y, z * rv.x - x * rv.z, x * rv.y - y * rv.x);
}

float
Vec3::length() const {
	float len;
	len = sqrt(x * x + y * y + z * z);
	return len;
}

Vec3
Vec3::norm() const {                            //单位向量
	return *this * (1 / this->length());
}

Point3::Point3(const Point3 &orig) :
	Vec3(orig) { }

ScreenPoint3&
ScreenPoint3::operator=(const ScreenPoint3 &orig) {
	this->x = orig.x;
	this->y = orig.y;
	this->z = orig.z;
	return *this;
}

TexturePoint2& 
TexturePoint2::operator=(const TexturePoint2 &orig) {
	this->x = orig.x;
	this->y = orig.y;
	return *this;
}

/*Point2::Point2(const Point2 &orig) :
	Point2(orig) { }*/

/*ostream &print(ostream &os, const Vec3 &a) {
os << a.x << " " << a.y << " " << a.z << endl;
return os;
}*/