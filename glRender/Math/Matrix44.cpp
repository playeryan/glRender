#include "Matrix44.h"

Matrix44::Matrix44(const Matrix44 &orig) {
	memcpy(&this->m[0], &orig.m[0], 16 * sizeof(float));
}

Matrix44::Matrix44(float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float m, float n, float o, float p) {
	this->m[0] = a; this->m[1] = b; this->m[2] = c; this->m[3] = d;
	this->m[4] = e; this->m[5] = f; this->m[6] = g; this->m[7] = h;
	this->m[8] = i; this->m[9] = j; this->m[10] = k; this->m[11] = l;
	this->m[12] = m; this->m[13] = n; this->m[14] = o; this->m[15] = p;
}

Matrix44::Matrix44(float mat[16]) 
{
	m[0] = mat[0];		m[1] = mat[1];		m[2] = mat[2];		m[3] = mat[3];
	m[4] = mat[4];		m[5] = mat[5];		m[6] = mat[6];		m[7] = mat[7];
	m[8] = mat[8];		m[9] = mat[9];		m[10] = mat[10];	m[11] = mat[11];
	m[12] = mat[12];	m[13] = mat[13];	m[14] = mat[14];	m[15] = mat[15];
}


Matrix44&
Matrix44::operator=(const Matrix44 &that) {
	this->m[0] = that.m[0];	this->m[1] = that.m[1]; this->m[2] = that.m[2]; this->m[3] = that.m[3];
	this->m[4] = that.m[4]; this->m[5] = that.m[5]; this->m[6] = that.m[6]; this->m[7] = that.m[7];
	this->m[8] = that.m[8]; this->m[9] = that.m[9]; this->m[10] = that.m[10]; this->m[11] = that.m[11];
	this->m[12] = that.m[12]; this->m[13] = that.m[13]; this->m[14] = that.m[14]; this->m[15] = that.m[15];

	return *this;
}

Matrix44&
Matrix44::operator+=(const Matrix44 &rm) {
	this->m[0] += rm.m[0]; this->m[1] += rm.m[1]; this->m[2] += rm.m[2]; this->m[3] += rm.m[3];
	this->m[4] += rm.m[4]; this->m[5] += rm.m[5]; this->m[6] += rm.m[6]; this->m[7] += rm.m[7];
	this->m[8] += rm.m[8]; this->m[9] += rm.m[9]; this->m[10] += rm.m[10]; this->m[11] += rm.m[11];
	this->m[12] += rm.m[12]; this->m[13] += rm.m[13]; this->m[14] += rm.m[14]; this->m[15] += rm.m[15];
	return *this;
}

Matrix44&
Matrix44::operator-=(const Matrix44 &rm) {
	this->m[0] -= rm.m[0]; this->m[1] -= rm.m[1]; this->m[2] -= rm.m[2]; this->m[3] -= rm.m[3];
	this->m[4] -= rm.m[4]; this->m[5] -= rm.m[5]; this->m[6] -= rm.m[6]; this->m[7] -= rm.m[7];
	this->m[8] -= rm.m[8]; this->m[9] -= rm.m[9]; this->m[10] -= rm.m[10]; this->m[11] -= rm.m[11];
	this->m[12] -= rm.m[12]; this->m[13] -= rm.m[13]; this->m[14] -= rm.m[14]; this->m[15] -= rm.m[15];
	return *this;
}

Matrix44&
Matrix44::operator*=(const Matrix44 &rm) {
	/*int i, j, k;
	Matrix44 tmp;
	for (i = 0; i < 4; ++i) {
	for (j = 0; j < 4; ++j) {
	for (k = 0; k < 4; ++k)
	tmp.m[i][j] += this->m[i][k] * rm.m[k][j];
	}
	}
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j) {
	m[i][j] = tmp.m[i][j];
	}
	return *this;*/

	Matrix44 tmp;
	tmp.m[0] = this->m[0] * rm.m[0] + this->m[1] * rm.m[4] + this->m[2] * rm.m[8] + this->m[3] * rm.m[12];
	tmp.m[1] = this->m[0] * rm.m[1] + this->m[1] * rm.m[5] + this->m[2] * rm.m[9] + this->m[3] * rm.m[13];
	tmp.m[2] = this->m[0] * rm.m[2] + this->m[1] * rm.m[6] + this->m[2] * rm.m[10] + this->m[3] * rm.m[14];
	tmp.m[3] = this->m[0] * rm.m[3] + this->m[1] * rm.m[7] + this->m[2] * rm.m[11] + this->m[3] * rm.m[15];

	tmp.m[4] = this->m[4] * rm.m[0] + this->m[5] * rm.m[4] + this->m[6] * rm.m[8] + this->m[7] * rm.m[12];
	tmp.m[5] = this->m[4] * rm.m[1] + this->m[5] * rm.m[5] + this->m[6] * rm.m[9] + this->m[7] * rm.m[13];
	tmp.m[6] = this->m[4] * rm.m[2] + this->m[5] * rm.m[6] + this->m[6] * rm.m[10] + this->m[7] * rm.m[14];
	tmp.m[7] = this->m[4] * rm.m[3] + this->m[5] * rm.m[7] + this->m[6] * rm.m[11] + this->m[7] * rm.m[15];

	tmp.m[8] = this->m[8] * rm.m[0] + this->m[9] * rm.m[4] + this->m[10] * rm.m[8] + this->m[11] * rm.m[12];
	tmp.m[9] = this->m[8] * rm.m[1] + this->m[9] * rm.m[5] + this->m[10] * rm.m[9] + this->m[11] * rm.m[13];
	tmp.m[10] = this->m[8] * rm.m[2] + this->m[9] * rm.m[6] + this->m[10] * rm.m[10] + this->m[11] * rm.m[14];
	tmp.m[11] = this->m[8] * rm.m[3] + this->m[9] * rm.m[7] + this->m[10] * rm.m[11] + this->m[11] * rm.m[15];

	tmp.m[12] = this->m[12] * rm.m[0] + this->m[13] * rm.m[4] + this->m[14] * rm.m[8] + this->m[15] * rm.m[12];
	tmp.m[13] = this->m[12] * rm.m[1] + this->m[13] * rm.m[5] + this->m[14] * rm.m[9] + this->m[15] * rm.m[13];
	tmp.m[14] = this->m[12] * rm.m[2] + this->m[13] * rm.m[6] + this->m[14] * rm.m[10] + this->m[15] * rm.m[14];
	tmp.m[15] = this->m[12] * rm.m[3] + this->m[13] * rm.m[7] + this->m[14] * rm.m[11] + this->m[15] * rm.m[15];

	this->m[0] = tmp.m[0];	this->m[1] = tmp.m[1];	this->m[2] = tmp.m[2];	this->m[3] = tmp.m[3];
	this->m[4] = tmp.m[4];	this->m[5] = tmp.m[5];	this->m[6] = tmp.m[6];	this->m[7] = tmp.m[7];
	this->m[8] = tmp.m[8];	this->m[9] = tmp.m[9];	this->m[10] = tmp.m[10];	this->m[11] = tmp.m[11];
	this->m[12] = tmp.m[12];	this->m[13] = tmp.m[13];	this->m[14] = tmp.m[14];	this->m[15] = tmp.m[15];
	return *this;
}

Matrix44&
Matrix44::operator*=(float f) {
	this->m[0] *= f;		this->m[1] *= f;		this->m[2] *= f;		this->m[3] *= f;
	this->m[4] *= f;		this->m[5] *= f;		this->m[6] *= f;		this->m[7] *= f;
	this->m[8] *= f;		this->m[9] *= f;		this->m[10] *= f;		this->m[11] *= f;
	this->m[12] *= f;		this->m[13] *= f;		this->m[14] *= f;		this->m[15] *= f;
	return *this;
}

Matrix44
Matrix44::operator+(const Matrix44 &rm) {
	Matrix44 tmp = *this;
	tmp += rm;
	return tmp;
}

Matrix44
Matrix44::operator-(const Matrix44 &rm) {
	Matrix44 tmp = *this;
	tmp -= rm;
	return tmp;
}

Matrix44
Matrix44::operator*(const Matrix44 &rm) {
	Matrix44 tmp = *this;
	tmp *= rm;
	return tmp;
}

Matrix44
Matrix44::operator*(float f) {
	Matrix44 tmp = *this;
	tmp *= f;
	return tmp;
}

//用于从屏幕像素坐标得到像素的观察坐标系下坐标
//此时ordi的x,y分量是像素的屏幕坐标;z分量是像素点在观察坐标系z分量的负倒数
Point4 
Matrix44::operator*(ScreenPoint3 ordi) {
	Point4 r = { ordi.x * 1.0f, ordi.y * 1.0f, 0.0f, 1.0f};   //w分量是否设为0？
	r = (*this) * r;
	r.z = -1 / ordi.z;
	r.w = 1;
	return r;
}

Vec4
Matrix44::operator*(const Vec4 u) {
	Vec4 r;
	r.x = m[0] * u.x + m[1] * u.y + m[2] * u.z;
	r.y = m[4] * u.x + m[5] * u.y + m[6] * u.z;
	r.z = m[8] * u.x + m[9] * u.y + m[10] * u.z;
	r.w = m[12] * u.x + m[13] * u.y + m[14] * u.z;
	return r;
}

Point4
Matrix44::operator*(const Point4 p) {
	//printf("before matrix operator *, point: (%f, %f, %f, %f)\n", p.x, p.y, p.z, p.w);
	//Mprint(*this);
	Point4 r;
	r.x = m[0] * p.x + m[1] * p.y + m[2] * p.z + m[3];
	r.y = m[4] * p.x + m[5] * p.y + m[6] * p.z + m[7];
	r.z = m[8] * p.x + m[9] * p.y + m[10] * p.z + m[11];
	r.w = m[12] * p.x + m[13] * p.y + m[14] * p.z + m[15];
	//printf("after matrix operator *, point: (%f, %f, %f, %f)\n", r.x, r.y, r.z, r.w);
	return r;
}

Point4 
Matrix44::operator^(const Point4 p) {
	Point4 r;
	r.x = m[0] * p.x + m[1] * p.y + m[2] * p.z + m[3];
	r.y = m[4] * p.x + m[5] * p.y + m[6] * p.z + m[7];
	r.z = m[8] * p.x + m[9] * p.y + m[10] * p.z + m[11];
	r.w = m[12] * p.x + m[13] * p.y + m[14] * p.z + m[15] * p.w;
	return r;
}

//功能性相关的成员函数

//设置观察平截头体；参数near、far并非坐标值，而是具体距离，为正数
Matrix44&
Matrix44::Frustum(float l, float r, float b, float t, float n, float f)
{
	Matrix44 tmp(
		2.0 * n / (r - l),	0.0,				/*(r + l) / (r - l)*/0.0,	0.0,
		0.0,				2.0 * n / (t - b),	/*(t + b) / (t - b)*/0.0,	0.0,
		0.0,				0.0,				-(f + n) / (f - n),			-2.0 * f * n / (f - n),
		0.0,				0.0,				-1.0,						0.0
		);
	this->operator*=(tmp);
	return *this;
}

//设置正投影矩阵,参数依次为左、右、下、上、近、远平面坐标
Matrix44&
Matrix44::Ortho(float l, float r, float b, float t, float n, float f) {
	Matrix44 tmp(
		2. / (r - l), 0., 0., -(r + l) / (r - l),
		0., 2. / (t - b), 0., -(t + b) / (t - b),
		0., 0., -2.0 / (f - n), -(f + n) / (f - n),
		0., 0., 0., 1.0
		);
	this->operator*=(tmp);
	return *this;
}

Matrix44&
Matrix44::viewport(float xl, float xr, float yb, float yt)               //设置规范立方体到屏幕视口的变换矩阵
{
	Matrix44 tmp((xr - xl) / 2, 0., 0., (xr + xl) / 2,
		0., (yt - yb) / 2, 0., (yt + yb) / 2,
		0., 0., 0.5, 0.5,
		0., 0., 0., 1.);
	this->operator*=(tmp);
	return *this;
}


//求余子式值
float
Matrix44::GetSubDeterminant(const int &i, const int &j) {
	float r = 0, tmp[9];
	int p, q, k = 0;
	for (p = 0; p < 4; ++q) {
		if (p == i)
			continue;
		for (q = 0; q < 4; ++q) {
			if (q == j)
				continue;
			tmp[k] = this->m[p * 4 + q];
			++k;
		}
	}
	r += (tmp[0] * tmp[4] * tmp[8] + tmp[1] * tmp[5] * tmp[6] + tmp[2] * tmp[3] * tmp[7]);
	r -= (tmp[2] * tmp[4] * tmp[6] + tmp[0] * tmp[5] * tmp[7] + tmp[1] * tmp[3] * tmp[8]);
	return r;
}

//求行列式值
float
Matrix44::GetDeterminant() {
	float r = 0;
	r += this->m[0] * GetSubDeterminant(0, 0);
	r -= this->m[1] * GetSubDeterminant(0, 1);
	r += this->m[2] * GetSubDeterminant(0, 2);
	r -= this->m[3] * GetSubDeterminant(0, 3);
	return r;
}

//求转置矩阵
Matrix44
Matrix44::GetTranspose() {
	Matrix44 r;
	int i, j;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			r.m[j * 4 + i] = this->m[i * 4 + j];
		}
	}
	return r;
}

//求矩阵的逆
Matrix44
Matrix44::Inverse() {
	// 用高斯-约当列主元消元法求逆

	const float EPSILON = 1.0e-6;
	const float SMALL = 1.0e-3;

	float aa[4][4];
	bool usedAsPivot[4];
	int rowIndex[4];
	int colIndex[4];
	int cs = 0;


	for (int j = 0; j < 4; j++)
		usedAsPivot[j] = false;

	memcpy(aa, this->m, 4 * 4 * sizeof(float));

	for (int i = 0; i < 4; i++)
	{
		// 寻找最大元素作为主元

		float big = 0.;
		int prow = -1;
		int pcol = -1;
		for (int j = 0; j < 4; j++)
		{
			if (!usedAsPivot[j])
			{
				for (int k = 0; k < 4; k++)
				{
					if (!usedAsPivot[k] && fabs(aa[j][k]) > big)
					{
						big = fabs(aa[j][k]);
						prow = j;
						pcol = k;
					}
				}
			}
		}

		// 如果最大元素小于10^-6,可认为所有元素全为0，矩阵不可逆

		if (fabs(big) <= EPSILON)
			return *this;

		// 标识已经被当做主元使用的列

		usedAsPivot[pcol] = true;

		// 进行行变换，将主元换至主对角线上

		if (prow != pcol)
		{
			for (int m = 0; m < 4; m++)
			{
				std::swap(aa[prow][m], aa[pcol][m]);
			}
		}

		if (fabs(aa[pcol][pcol]) <= EPSILON)
			return *this;

		rowIndex[i] = prow;
		colIndex[i] = pcol;

		// 使用主元，对主元行约分

		float divide = aa[pcol][pcol];
		aa[pcol][pcol] = 1.;

		for (int m = 0; m < 4; m++)
		{
			aa[pcol][m] /= divide;
		}

		// 其他行循环减去主元

		for (int mm = 0; mm < 4; mm++)
		{
			if (mm != pcol)
			{
				float tmp = aa[mm][pcol];
				aa[mm][pcol] = 0.;
				for (int m = 0; m < 4; m++) {
					aa[mm][m] -= tmp * aa[pcol][m];
				}
			}
		}
	}

	// 整理已经做过的列变换:

	for (int m = 4 - 1; m >= 0; m--)
	{
		if (rowIndex[m] != colIndex[m])
		{
			for (int k = 0; k < 4; k++)
			{
				std::swap(aa[k][rowIndex[m]], aa[k][colIndex[m]]);
			}
		}
	}

	// 完成

	memcpy(this->m, aa, 4 * 4 * sizeof(float));	// 将aa数组复制给对象的成员

	return *this;
}

Matrix44 &
Matrix44::LoadIdentity()
{
	m[0] = m[5] = m[10] = m[15] = 1.;
	m[1] = m[2] = m[3] = 0.;
	m[4] = m[6] = m[7] = 0.;
	m[8] = m[9] = m[11] = 0.;
	m[12] = m[13] = m[14] = 0.;
	return *this;
}


//三维平移
Matrix44&
Matrix44::Translatef(float tx, float ty, float tz) {
	Matrix44 tmp(1., 0., 0., tx,
		0., 1., 0., ty,
		0., 0., 1., tz,
		0., 0., 0., 1.);
	this->operator*=(tmp);
	return *this;
}

//三维旋转(使用四元数)，参数分别为角度、轴向量
Matrix44&
Matrix44::Rotatef(float deg, float vx, float vy, float vz) {
	// 将轴向量化为单位向量
	Matrix44 m;
	float qsin = sinf(deg * 0.5f);
	float qcos = cosf(deg * 0.5f);
	Vec4 vec = { vx, vy, vz, 0};
	float w = qcos;
	vec.norm();
	float x = vec.x * qsin;
	float y = vec.y * qsin;
	float z = vec.z * qsin;
	m.m[0] = 1 - 2 * y * y - 2 * z * z;
	m.m[1] = 2 * x * y - 2 * w * z;
	m.m[2] = 2 * x * z + 2 * w * y;
	m.m[4] = 2 * x * y + 2 * w * z;
	m.m[5] = 1 - 2 * x * x - 2 * z * z;
	m.m[6] = 2 * y * z - 2 * w * x;
	m.m[8] = 2 * x * z - 2 * w * y;
	m.m[9] = 2 * y * z + 2 * w * x;
	m.m[10] = 1 - 2 * x * x - 2 * y * y;
	m.m[3] = m.m[7] = m.m[11] = 0.0f;
	m.m[12] = m.m[13] = m.m[14] = 0.0f;
	m.m[15] = 1.0f;

	this->operator*=(m);
	return *this;
}

//三维缩放
Matrix44&
Matrix44::Scalef(float sx, float sy, float sz) {
	Matrix44 tmp(
		sx, 0., 0., 0.,
		0., sy, 0., 0.,
		0., 0., sz, 0.,
		0., 0., 0., 1.);

	this->operator*=(tmp);
	return *this;
}

//参数依次为相机位置，方向，向上向量
Matrix44&
Matrix44::LookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	Vec4 n(eyex - centerx, eyey - centery, eyez - centerz);
	n.norm();
	Vec4 v(upx, upy, upz, 0);
	v.norm();
	Vec4 u = v ^ n;
	u.norm();
	v = n ^ u;
	v.norm();

	Matrix44 tmp(
				u.x, u.y, u.z, 0.0,
				v.x, v.y, v.z, 0.0,
				n.x, n.y, n.z, 0.0,
				0.0, 0.0, 0.0, 1.0);

	tmp.Translatef(-eyex, -eyey, -eyez);

	this->operator*=(tmp);
	return *this;
}

Matrix44& Matrix44::LookAt(Vec4 eyePosition, Vec4 nVector, Vec4 upVector)
{
	Vec4 n = nVector;
	n.norm();
	Vec4 v = upVector;
	v.norm();
	Vec4 u = v ^ n;
	u.norm();
	v = n ^ u;
	v.norm();

	Matrix44 tmp(
		u.x, u.y, u.z, 0.0,
		v.x, v.y, v.z, 0.0,
		n.x, n.y, n.z, 0.0,
		0.0, 0.0, 0.0, 1.0);

	tmp.Translatef(-eyePosition.x, -eyePosition.y, -eyePosition.z);

	this->operator*=(tmp);
	return *this;
}

//通过视场角等参数设置观察平截头体
Matrix44 &
Matrix44::Perspective(float fovy, float aspect, float near, float far) {
	float rad = (PI / 180.0f) * (fovy / 2.0f);
	float t = tanf(rad);
	float top = near * t;
	float bottom = -top;
	float right = aspect * top;
	float left = -right;

	this->Frustum(left, right, bottom, top, near, far);
	return *this;
}

//指定矩阵中的某一个值
Matrix44&
Matrix44::SetElement(int i, int j, float value) {
	if (i < 0 || i > 3)
		return *this;
	if (j < 0 || j >3)
		return *this;
	this->m[i * 4 + j] = value;
	return *this;
}


//非成员函数
void Mprint(const Matrix44 &lm) {
	printf("\t%8.2f  %8.2f  %8.2f  %8.2f\n", lm.m[0], lm.m[1], lm.m[2], lm.m[3]);
	printf("\t%8.2f  %8.2f  %8.2f  %8.2f\n", lm.m[4], lm.m[5], lm.m[6], lm.m[7]);
	printf("\t%8.2f  %8.2f  %8.2f  %8.2f\n", lm.m[8], lm.m[9], lm.m[10], lm.m[11]);
	printf("\t%8.2f  %8.2f  %8.2f  %8.2f\n", lm.m[12], lm.m[13], lm.m[14], lm.m[15]);
}

