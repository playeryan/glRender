#pragma once
#ifndef CAMERA_H
#define	CAMERA_H

#include "../Math/Matrix44.h"
#include "../Math/Vec3.h"

class Camera
{
public:
	Camera(int width, int height);
	~Camera();

	void setCamera(Point4 eye, Vec4 target, Vec4 up);
	void setProjectionMatrix(float fov, float ratio, float nearClip, float farClip);
	Matrix44 getMVPMatrix();
	Matrix44 getModelMatrix();
	Matrix44 getViewMatrix();
	Matrix44 getNormTransformMatrix();
	void onKeyBoard(unsigned char key);
	void onMouseMotion(int x, int y);
	void setMousePosition(int x, int y);
	Point4 getEyePosition();
	Vec4 getLookingDirection();

	inline void setDistanceFactor(float factor)
	{
		m_distanceFactor = factor > 0.0f ? factor : -factor;
		printf("Camera distance factor: %f\n", m_distanceFactor);
	}
	inline float getDistanceFactor() 
	{
		return m_distanceFactor;
	}

public:
	int			m_windowWidth;
	int			m_windowHeight;
	Point4		m_eyePosition;
	//右手坐标系，实际坐标系正向向量nVector与targetVector反向
	Vec4		m_nVector;
	Vec4		m_upVector;
	int			m_mouseX;
	int			m_mouseY;
	Matrix44	m_projMatrix;
	float		m_distanceFactor;
};

#endif // !CAMERA_H

