#include "Camera.h"

//static const float moveSpeed = 10.0f;

Camera::Camera(int width, int height)
	: m_windowWidth(width)
	, m_windowHeight(height)
	, m_eyePosition(Point4(0, 0, 0, 1))
	, m_worldPos(Point4(0, 0, 0, 1))
	, m_scale(Vec3(1, 1, 1))
	, m_nVector(Vec4(0, 0, -1, 0))
	, m_upVector(Vec4(0, 1, 0, 0))
	, m_mouseX(0)
	, m_mouseY(0)
	, m_distanceFactor(0.0)
{}

Camera::~Camera()
{
}

void Camera::setCamera(Point4 eye, Vec4 target, Vec4 up)
{
	m_eyePosition = eye;
	m_nVector = Vec4::Zero - target;
	m_upVector = up;
}

void Camera::setWorldPos(Point4 pos)
{
	m_worldPos = pos;
}

void Camera::setScale(Vec3 factor)
{
	m_scale = factor;
}

void Camera::setProjectionMatrix(float fov, int width, int height, float nearClip, float farClip)
{
	float ratio = (float)width / (float)height;
	m_projMatrix.LoadIdentity();
	m_projMatrix.Perspective(fov, ratio, nearClip, farClip);
}

void Camera::recoverModelTransform()
{
	m_worldPos = Point4(0, 0, 0, 1);
	m_scale = Vec3(1, 1, 1);
}

Matrix44 Camera::getMVPMatrix()
{
	Matrix44 result = m_projMatrix * getViewMatrix() * getModelMatrix();
	return result;
}

Matrix44 Camera::getModelMatrix()
{
	Matrix44 modelMatrix;
	modelMatrix.LoadIdentity();
	modelMatrix.Scalef(m_scale.x, m_scale.y, m_scale.z);
	// 等以后需要时再加入旋转变换/
	modelMatrix.Translatef(m_worldPos.x, m_worldPos.y, m_worldPos.z);

	return modelMatrix;
}

Matrix44 Camera::getViewMatrix()
{
	Matrix44 viewMatrix;
	viewMatrix.LoadIdentity();
	viewMatrix.LookAt(m_eyePosition, m_nVector, m_upVector);

	return viewMatrix;
}

Matrix44 Camera::getNormTransformMatrix()
{
	// 光照计算放在世界坐标下计算，故直接使用单位矩阵/
	//Matrix44 viewMatrix;
	//viewMatrix.LoadIdentity();
	//viewMatrix.LookAt(m_eyePosition, m_nVector, m_upVector);

	//viewMatrix.Inverse();
	//Matrix44 result = viewMatrix.GetTranspose();

	Matrix44 result;
	return result.LoadIdentity();
}

void Camera::onKeyBoard(int key, float deltaTime)
{
	if (key == GLFW_KEY_W)
	{
		m_eyePosition = m_eyePosition - (m_nVector * m_distanceFactor * deltaTime);
	}
	if (key == GLFW_KEY_S)
	{
		m_eyePosition = m_eyePosition + (m_nVector * m_distanceFactor * deltaTime);
	}
	if (key == GLFW_KEY_A)
	{
		Vec4 left = m_nVector ^ m_upVector;
		left.norm();
		left = left * m_distanceFactor * deltaTime;
		m_eyePosition = m_eyePosition + left;

	}
	if (key == GLFW_KEY_D)
	{
		Vec4 right = m_upVector ^ m_nVector;
		right.norm();
		right = right * m_distanceFactor * deltaTime;
		m_eyePosition = m_eyePosition + right;
	}
	if (key == GLFW_KEY_I)
	{
		m_eyePosition.y += m_distanceFactor * deltaTime;
	}
	if (key == GLFW_KEY_K)
	{
		m_eyePosition.y -= m_distanceFactor * deltaTime;
	}
}

void Camera::onMouseMotion(double x, double y)
{
	float scale = 0.1f;

	double deltaX = x - m_mouseX, deltaY = y - m_mouseY;
	m_mouseX = x;
	m_mouseY = y;

	float radianX = deltaX * scale * PI / 180.0f, radianY = deltaY * scale * PI / 180.0f;
	Matrix44 rotateX, rotateY;

	Vec4 originUpVector = Vec4(0, 1, 0, 0);
	rotateX.LoadIdentity();
	rotateX.Rotatef(radianX, originUpVector.x, originUpVector.y, originUpVector.z);
	Vec4 nView = rotateX * m_nVector;
	nView.norm();
	Vec4 axisHorizontal = originUpVector ^ nView;
	axisHorizontal.norm();

	rotateY.LoadIdentity();
	rotateY.Rotatef(radianY, axisHorizontal.x, axisHorizontal.y, axisHorizontal.z);
	m_nVector = rotateY * nView;
	m_nVector.norm();

	m_upVector = m_nVector ^ axisHorizontal;
}

void Camera::setMousePosition(double x, double y)
{
	m_mouseX = x;
	m_mouseY = y;
}

Point4 Camera::getEyePosition()
{
	return m_eyePosition;
}

Vec4 Camera::getLookingDirection()
{
	Vec4 targetVector = Vec4::Zero - m_nVector;

	return targetVector;
}
