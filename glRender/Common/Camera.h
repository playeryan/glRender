#pragma once
#ifndef CAMERA_H
#define	CAMERA_H

#include <GLFW\glfw3.h>
#include "../Math/Matrix44.h"
#include "../Math/Vec3.h"

class Camera
{
public:
	Camera(int width, int height);
	~Camera();

	void setCamera(Point4 eye, Vec4 target, Vec4 up);
	void setWorldPos(Point4 pos);
	void setScale(Vec3 factor);
	void setProjectionMatrix(float fov, int width, int height, float nearClip, float farClip);
	void recoverModelTransform();
	Matrix44 getMVPMatrix();
	Matrix44 getModelMatrix();
	Matrix44 getViewMatrix();
	Matrix44 getNormTransformMatrix();
	void onKeyBoard(int key, float deltaTime); // deltaTime参数使得不同帧数下，相机移动速度保持固定/
	void onMouseMotion(double x, double y);
	void setMousePosition(double x, double y);
	Point4 getEyePosition();
	Vec4 getLookingDirection();

	inline void setDistanceFactor(float factor)
	{
		m_distanceFactor = factor;
		printf("Camera distance factor: %f\n", m_distanceFactor);
	}

public:
	int			m_windowWidth;
	int			m_windowHeight;
	Point4		m_eyePosition;
	Point4		m_worldPos;
	Vec3		m_scale;
	//右手坐标系，实际坐标系正向向量nVector与targetVector反向
	Vec4		m_nVector;
	Vec4		m_upVector;
	double		m_mouseX;
	double		m_mouseY;
	Matrix44	m_projMatrix;
	float		m_distanceFactor;
};

#endif // !CAMERA_H

