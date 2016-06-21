#include <windows.h>
#include <stdlib.h>
#include "BaseApp.h"

static BaseApp* appDelegate = NULL;

BaseApp::BaseApp(const char* name)
	: m_pCamera(NULL)
	, m_deltaTime(0.0)
	, m_lastTime(0.0)
	, m_elapsedTime(0.0f)
	, m_frameCount(0)
	, m_FPS(0.0)
	, m_appName(name)
	, m_drawMode(1)
{
	for (size_t i = 0; i < 1024; i++)
	{
		m_keyState.push_back(false);
	}
}

BaseApp::~BaseApp()
{
	SAFE_DELETE_POINTER(m_pCamera);
}

void BaseApp::appMain(int argc, char** argv, BaseApp* appImplement)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, m_appName.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << std::endl;

	m_pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	instantiateAppDelegate(appImplement);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!init())
	{
		return;
	}

	//// render loop
	m_lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		glfwPollEvents();
		movementEventPoll();
		display();
		fpsCalculate();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return;
}

void BaseApp::fpsCalculate()
{
	m_frameCount++;
	m_elapsedTime += m_deltaTime;
	// 检测是否完整度过1秒钟
	char fpsChar[20] = { 0 };
	if (m_elapsedTime >= 1.0f)
	{
		// 计算帧率
		m_FPS = m_frameCount / m_elapsedTime;
		// 写入帧率
		sprintf(fpsChar, "%f", m_FPS);
		std::string fpsString(fpsChar);
		fpsString += '\n';
		printf(fpsString.c_str());
		// 清零，重新计时
		m_elapsedTime = 0.0f;
		m_frameCount = 0;
	}
}

void BaseApp::movementEventPoll()
{
	keyboardFunc(GLFW_KEY_W);
	keyboardFunc(GLFW_KEY_A);
	keyboardFunc(GLFW_KEY_S);
	keyboardFunc(GLFW_KEY_D);
	keyboardFunc(GLFW_KEY_I);
	keyboardFunc(GLFW_KEY_K);

	keyboardFunc(GLFW_KEY_1);
	keyboardFunc(GLFW_KEY_2);
	keyboardFunc(GLFW_KEY_3);
	keyboardFunc(GLFW_KEY_4);
	keyboardFunc(GLFW_KEY_5);
}

void BaseApp::keyboardFunc(int key)
{
	if (m_keyState[key])
	{
		m_pCamera->onKeyBoard(key, m_deltaTime);
		switchDrawMode(key);
	}
}

void BaseApp::mouseFunc(double x, double y)
{
	m_pCamera->setMousePosition(x, y);
}

void BaseApp::mouseButtonFunc(double x, double y)
{
	m_pCamera->onMouseMotion(x, y);
}

void BaseApp::switchDrawMode(int key)
{
	if (key == GLFW_KEY_1)
	{
		m_drawMode = 1;
	}
	if (key == GLFW_KEY_2)
	{
		m_drawMode = 2;
	}
	if (key == GLFW_KEY_3)
	{
		m_drawMode = 3;
	}
	if (key == GLFW_KEY_4)
	{
		m_drawMode = 4;
	}
	if (key == GLFW_KEY_5)
	{
		m_drawMode = 5;
	}
}

void instantiateAppDelegate(BaseApp* appImplement)
{
	appDelegate = appImplement;
}

void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	if (action == GLFW_PRESS)
	{
		appDelegate->m_keyState[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		appDelegate->m_keyState[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double x, double y)
{
	// 按住鼠标右键时才控制相机/
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		appDelegate->mouseButtonFunc(x, y);
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS)
	{
		double x = 0.0, y = 0.0;
		glfwGetCursorPos(window, &x, &y);

		appDelegate->mouseFunc(x, y);
	}
}
