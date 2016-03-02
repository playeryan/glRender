#pragma once
#ifndef BASEAPP_H
#define BASEAPP_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
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
	void changeWindowSize(int w, int h);
	void keyboardFunc(unsigned char key, int x, int y);
	void mouseFunc(int button, int state, int x, int y);
	void motionFunc(int x, int y);

// class member region start
public:
	Camera*			m_pCamera;
private:
	float			m_lastTime;
	float			m_elapsedTime;
	unsigned int	m_frameCount;
	float			m_FPS;
	std::string		m_appName;
// class member region end
};

static void instantiateAppDelegate(BaseApp* appImplement);
static void changeSize(int w, int h);
static void render();
static void keyboardCallback(unsigned char key, int x, int y);
static void mouseCallback(int button, int state, int x, int y);
static void motionCallback(int x, int y);

#endif // !BASEAPP_H

