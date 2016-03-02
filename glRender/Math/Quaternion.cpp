#include "Quaternion.h"

const Quat Quat::Zero   (0, 0, 0, 0);
const Quat Quat::XAxis  (1, 0, 0, 0);
const Quat Quat::YAxis  (0, 1, 0, 0);
const Quat Quat::ZAxis  (0, 0, 1, 0);
const Quat Quat::UnitOne(0, 0, 0, 1);


Quat::Quat(const Quat &orig) :
	x(orig.x), y(orig.y), z(orig.z), w(orig.w) { }



//四元数加法
Quat
Quat::operator+(const Quat &rq) const {
	return Quat(x + rq.x, y + rq.y, z + rq.z);
}

//四元数减法
Quat
Quat::operator-(const Quat &rq) const {
	return Quat(x - rq.x, y - rq.y, z - rq.z);
}

//四元数乘法
Quat 
Quat::operator*(const Quat &rq) const {
	Quat result;
	result.w = this->w * rq.w - (this->x * rq.x + this->y * rq.y + this->z * rq.z);
	result.x = this->w * rq.x + rq.w * this->x + this->y * rq.z - this->z * rq.y;
	result.y = this->w * rq.y + rq.w * this->y + this->z * rq.x - this->x * rq.z;
	result.z = this->w * rq.z + rq.w * this->z + this->x * rq.y - this->y * rq.x;
	return result;
}

float
Quat::length() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

Quat
Quat::norm() {                                   //单位向量（仅对虚数向量部分归一化）
	float len = sqrt(x * x + y * y + z * z);
	if (len > 0.)
	{
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}
	return *this;
}

Quat 
Quat::conjugate() {                      //共轭四元数
	return Quat(-x, -y, -z, w);
}

Quat 
Quat::inverse() {                        //四元数的逆；若四元数已归一化，则其逆等于其共轭四元数，无需此函数计算
	Quat result;
	float len_square = x * x + y * y + z * z + w * w;
	result.w =    this->w / len_square;
	result.x = (-this->x) / len_square;
	result.y = (-this->y) / len_square;
	result.z = (-this->z) / len_square;
	return result;
}

Vec4 
Quat::rotate(Vec4 p) {                    //旋转任意向量
	Vec4 r;
	float coe = p.x * this->x + p.y * this->y + p.z * this->z;
	Vec4 x_coe = { this->y * p.z - this->z * p.y, 
		           this->z * p.x - this->x * p.z, 
		           this->x * p.y - this->y * p.x };
	Vec4 xx_coe = { this->y * x_coe.z - this->z * x_coe.y, 
		            this->z * x_coe.x - this->x * x_coe.z, 
		            this->x * x_coe.y - this->y * x_coe.x };
	r.x = (this->w * this->w) * p.x + 2 * this->w * x_coe.x + xx_coe.x;
	r.y = (this->w * this->w) * p.y + 2 * this->w * x_coe.y + xx_coe.y;
	r.z = (this->w * this->w) * p.z + 2 * this->w * x_coe.z + xx_coe.z;
	return r;
}

Point4 
Quat::rotate(Point4 p) {
	Point4 r;
	float coe = p.x * this->x + p.y * this->y + p.z * this->z;
	Vec4 x_coe = { this->y * p.z - this->z * p.y,
		this->z * p.x - this->x * p.z,
		this->x * p.y - this->y * p.x };
	Vec4 xx_coe = { this->y * x_coe.z - this->z * x_coe.y,
		this->z * x_coe.x - this->x * x_coe.z,
		this->x * x_coe.y - this->y * x_coe.x };
	r.x = (this->w * this->w) * p.x + 2 * this->w * x_coe.x + xx_coe.x;
	r.y = (this->w * this->w) * p.y + 2 * this->w * x_coe.y + xx_coe.y;
	r.z = (this->w * this->w) * p.z + 2 * this->w * x_coe.z + xx_coe.z;
	return r;
}