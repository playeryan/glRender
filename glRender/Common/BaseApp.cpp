#include <windows.h>
#include <stdlib.h>
#include "BaseApp.h"

static BaseApp* appDelegate = NULL;

BaseApp::BaseApp(const char* name)
	: m_pCamera(NULL)
	, m_lastTime(0.0)
	, m_elapsedTime(0.0)
	, m_frameCount(0)
	, m_FPS(0.0)
	, m_appName(name)
{}

BaseApp::~BaseApp()
{
	SAFE_DELETE_POINTER(m_pCamera);
}

void BaseApp::appMain(int argc, char** argv, BaseApp* appImplement)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(m_appName.c_str());
	if (glewInit())
	{
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << std::endl;

	m_pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_lastTime = (float)timeGetTime();

	instantiateAppDelegate(appImplement);
	if (!init())
	{
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glutReshapeFunc(changeSize);
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutMainLoop();
}

void BaseApp::fpsCalculate()
{
	m_frameCount++;
	float currentTime = (float)timeGetTime();	// 结果为毫秒/
	float timeDelta = (currentTime - m_lastTime) * 0.001f;
	m_elapsedTime += timeDelta;
	// 检测是否完整度过1秒钟
	char fpsChar[20] = { 0 };
	if (m_elapsedTime >= 1.0f)
	{
		// 计算帧率
		m_FPS = (float)m_frameCount / m_elapsedTime;
		// 写入帧率
		sprintf(fpsChar, "%f", m_FPS);
		std::string fpsString(fpsChar);
		fpsString += '\n';
		printf(fpsString.c_str());
		// 清零，重新计时
		m_elapsedTime = 0.0f;
		m_frameCount = 0;
	}
	m_lastTime = currentTime;	
}

void BaseApp::changeWindowSize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void BaseApp::keyboardFunc(unsigned char key, int x, int y)
{
	m_pCamera->onKeyBoard(key);
}

void BaseApp::mouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		m_pCamera->setMousePosition(x, y);
	}
}

void BaseApp::motionFunc(int x, int y)
{
	m_pCamera->onMouseMotion(x, y);
}

void instantiateAppDelegate(BaseApp* appImplement)
{
	appDelegate = appImplement;
}

void changeSize(int w, int h)
{
	appDelegate->changeWindowSize(w, h);
}

void render()
{
	appDelegate->display();
}

void keyboardCallback(unsigned char key, int x, int y)
{
	appDelegate->keyboardFunc(key, x, y);
}

void mouseCallback(int button, int state, int x, int y)
{
	appDelegate->mouseFunc(button, state, x, y);
}

void motionCallback(int x, int y)
{
	appDelegate->motionFunc(x, y);
}
