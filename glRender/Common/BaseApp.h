#pragma once
#ifndef BASEAPP_H
#define BASEAPP_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "ShaderObject.h"
#include "Macros/glMacros.h"
#include "Math/Matrix44.h"
#include "Math/Vec4.h"
#include "Math/Vec3.h"

class BaseApp
{
public:
	BaseApp() = default;
	BaseApp(const char* name);
	~BaseApp();
	virtual bool init() = 0;
	virtual void display() = 0;

	virtual void appMain(int argc, char** argv, BaseApp* appImplement);

	void fpsCalculate();
	void movementEventPoll();
	void keyboardFunc(int key);
	void mouseFunc(double x, double y);
	void mouseButtonFunc(double x, double y);

	// class member region start
public:
	Camera*				m_pCamera;
	std::vector<bool>	m_keyState;
	float				m_deltaTime;
private:
	float				m_lastTime;
	float				m_elapsedTime;
	unsigned int		m_frameCount;
	float				m_FPS;
	std::string			m_appName;
	// class member region end
};

static void instantiateAppDelegate(BaseApp* appImplement);
static void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
static void mouseCallback(GLFWwindow* window, double x, double y);
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);

#endif // !BASEAPP_H

