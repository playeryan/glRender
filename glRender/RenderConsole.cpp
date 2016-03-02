#include <iostream>
#include <windows.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include "Macros/glMacros.h"
#include "Math\MathMacros.h"
#include "Math\Vec4.h"
#include "Model\Bitmap.h"
#include "Common\Texture.h"
#include "Common\BaseApp.h"
#include "Common\LightTechnique.h"
#include "Common\Light.h"
#include "Model\Mesh.h"

#pragma comment (lib, "assimp.lib")

using namespace std; 

Vec4	targetVector	=	Vec4(0, 0, -1, 0);
Vec4	upVector		=	Vec4(0, 1, 0, 0);
float	specularIntensity = 0.9f;
float	specularPower = 128;
// inherit from BaseApp
class GLrender	: public BaseApp
{
public:
	GLrender(const char* name)
		:	BaseApp(name)
		,	m_pGameEffect(NULL)
		,	m_pMesh(NULL)
	{
		m_dirLight.Color = Vec3(1.0f, 1.0f, 1.0f);
		m_dirLight.AmbientIntensity = 0.15f;
		m_dirLight.Direction = Vec4(0.0f, -1.0f, 0.0f, 0.0f);	// 平行光从天空竖直射向地面/
		m_dirLight.DiffuseIntensity = 0.2f;
	}
	~GLrender()
	{
		SAFE_DELETE_POINTER(m_pGameEffect);
		SAFE_DELETE_POINTER(m_pMesh);
	}

	bool init() override
	{
		m_pGameEffect = new LightTechnique();
		m_pGameEffect->loadShader();
		m_pGameEffect->setTextureSampler(0);

		m_pMesh = new Mesh();
		// 加载不同的模型文件/
		bool flag = m_pMesh->LoadMesh("Resource/crytek-sponza/sponza.obj");
		//bool flag = m_pMesh->LoadMesh("Resource/dabrovic-sponza/sponza.obj");
		//bool flag = m_pMesh->LoadMesh("Resource/head/head.obj");
		//bool flag = m_pMesh->LoadMesh("Resource/sibenik/sibenik.obj");

		Point3 sceneCenter = m_pMesh->GetSceneCenterPos();
		m_pCamera->setCamera(Point4(sceneCenter.x, sceneCenter.y, sceneCenter.z, 1.0f), targetVector, upVector);
		m_pCamera->setDistanceFactor(m_pMesh->getSuitableDistanceFactor());
		m_pCamera->setProjectionMatrix(75.0, 4 / 3, 1, 10000);

		return flag;
	}
	void display() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static float pointLightHeightParam = 0.0f;
		pointLightHeightParam += 1.0f;
		float lightPosParam = cosf(ToRadian(pointLightHeightParam)) * m_pCamera->getDistanceFactor() * 200;
		Point4 sceneCenterPos = Point4(m_pMesh->GetSceneCenterPos());

		PointLight pointLightArray[2];
		static float pointLightBaseX = sceneCenterPos.x;
		Point4 pointLightPos = Point4(pointLightBaseX + lightPosParam * 2.0f, sceneCenterPos.y * 1.2f, sceneCenterPos.z);
		pointLightArray[0].PointLightPos = Point4(pointLightPos.x, pointLightPos.y, pointLightPos.z);
		pointLightArray[0].Color = Vec3(0.5, 1.0, 0.0);
		pointLightArray[0].AmbientIntensity = 0.8f;
		pointLightArray[0].DiffuseIntensity = 0.5f;
		pointLightArray[0].AttenuationFactor.Constant = 1.0f;
		pointLightArray[0].AttenuationFactor.Linear = 1e-2f;
		pointLightArray[0].AttenuationFactor.Exp = 1e-4f;

		SpotLight spotLightArray[2];
		static float spotLightBaseHeight = sceneCenterPos.y;
		Point4 spotLightPos = Point4(sceneCenterPos.x, spotLightBaseHeight + lightPosParam, sceneCenterPos.z);
		spotLightArray[0].AmbientIntensity = 0.9f;
		spotLightArray[0].DiffuseIntensity = 0.8f;
		spotLightArray[0].Color = Vec3(0.0f, 0.0f, 1.0f);
		spotLightArray[0].PointLightPos = Point4(spotLightPos.x, spotLightPos.y, spotLightPos.z);
		spotLightArray[0].Direction = Vec4(0.0, -1.0, 0.0, 0.0);
		spotLightArray[0].AttenuationFactor.Constant = 1.0f;
		spotLightArray[0].AttenuationFactor.Linear = 1e-4f;
		spotLightArray[0].AttenuationFactor.Exp = 1e-5f;
		spotLightArray[0].CutOffAngle = 30.0f;

		// 光照计算放在世界坐标下/
		m_pGameEffect->setMVPMatrix(m_pCamera->getMVPMatrix());
		m_pGameEffect->setModelMatrix(m_pCamera->getModelMatrix());
		m_pGameEffect->setNormTransformMatrix(m_pCamera->getNormTransformMatrix());
		m_pGameEffect->setDirLightParams(m_dirLight);
		m_pGameEffect->setEyeWorldPos(m_pCamera->getEyePosition());
		m_pGameEffect->setSpecularIntensity(specularIntensity);
		m_pGameEffect->setSpecularPower(specularPower);
		m_pGameEffect->setPointLightsParams(1, pointLightArray);
		m_pGameEffect->setSpotLightsParams(1, spotLightArray);

		m_pMesh->Render();
		glutSwapBuffers();
		fpsCalculate();
	}
private:
	LightTechnique*		m_pGameEffect;
	DirectionalLight	m_dirLight;
	Mesh*				m_pMesh;
};

//---------------------------------------------------------------------
//
// main
//
void main(int argc, char** argv)
{
	GLrender* render = new GLrender(WINDOW_TITLE);
	render->appMain(argc, argv, render);

	delete render;
	return;
}
