#include <iostream>
#include <cstdio>
#include <windows.h>
#include <math.h>
#include <GL/glew.h>
#include "Macros/glMacros.h"
#include "Math\MathMacros.h"
#include "Math\Vec4.h"
#include "Model\Bitmap.h"
#include "Common\Texture.h"
#include "Common\BaseApp.h"
#include "Common\ShaderObject.h"
#include "Common\FrameBufferObject.h"
#include "Common\GeometryBufferObject.h"
#include "Common\Light.h"
#include "Model\Mesh.h"
#include "ShaderFiles\ShaderFiles.h"

using namespace std; 

Vec4	targetVector		=	Vec4(0, 0, -1, 0);
Vec4	upVector			=	Vec4(0, 1, 0, 0);
float	specularIntensity	=	0.9f;
float	specularPower		=	128;
int		pointLightNum		=	1;
int		spotLightNum		=	1;

// inherit from BaseApp
class GLrender	: public BaseApp
{
public:
	GLrender(const char* name)
		:	BaseApp(name)
		,	m_pGameLightEffect(NULL)
		,	m_pGameDirLightEffect(NULL)
		,	m_pGamePointLightEffect(NULL)
		,	m_pGameSpotLightEffect(NULL)
		,	m_pGameGBufferEffect(NULL)
		,	m_pGameStencilEffect(NULL)
		,	m_pGameShadowEffect(NULL)
		,	m_pMesh(NULL)
	{
		InitLights();
	}
	~GLrender()
	{
		SAFE_DELETE_POINTER(m_pGameLightEffect);
		SAFE_DELETE_POINTER(m_pGameDirLightEffect);
		SAFE_DELETE_POINTER(m_pGamePointLightEffect);
		SAFE_DELETE_POINTER(m_pGameSpotLightEffect);
		SAFE_DELETE_POINTER(m_pGameGBufferEffect);
		SAFE_DELETE_POINTER(m_pGameStencilEffect);
		SAFE_DELETE_POINTER(m_pGameShadowEffect);
		SAFE_DELETE_POINTER(m_pMesh);
	}

	bool init() override
	{
		if (!m_geometryBufferObject.Init(WINDOW_WIDTH, WINDOW_HEIGHT))
		{
			return false;
		}

		m_pGameLightEffect = new LightShader(LightVSFileName.c_str(), LightFSFileName.c_str());
		m_pGameLightEffect->Bind();

		m_pGameGBufferEffect = new GeometryBufferShader(GBufferVSFileName.c_str(), GBufferFSFileName.c_str());
		m_pGameGBufferEffect->Bind();
		m_pGameGBufferEffect->SetColorTextureUnit(0);

		m_pGameDirLightEffect = new DirLightShader(DirLightVSFileName.c_str(), DirLightFSFileName.c_str());
		m_pGameDirLightEffect->Bind();
		m_pGameDirLightEffect->setSpecularIntensity(specularIntensity);
		m_pGameDirLightEffect->setSpecularPower(specularPower);
		m_pGameDirLightEffect->setPositionTextureUnit(GeometryBuffer::GeometryBufferTextureType_Position);
		m_pGameDirLightEffect->setColorTextureUnit(GeometryBuffer::GeometryBufferTextureType_Diffuse);
		m_pGameDirLightEffect->setNormalTextureUnit(GeometryBuffer::GeometryBufferTextureType_Normal);
		m_pGameDirLightEffect->setDirLightParams(m_dirLight);
		m_pGameDirLightEffect->setScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		Matrix44 m;
		m.LoadIdentity();
		m_pGameDirLightEffect->setMVPMatrix(m);

		m_pGamePointLightEffect = new PointLightShader(PointLightVSFileName.c_str(), PointLightFSFileName.c_str());
		m_pGamePointLightEffect->Bind();
		m_pGamePointLightEffect->setSpecularIntensity(specularIntensity);
		m_pGamePointLightEffect->setSpecularPower(specularPower);
		m_pGamePointLightEffect->setPositionTextureUnit(GeometryBuffer::GeometryBufferTextureType_Position);
		m_pGamePointLightEffect->setColorTextureUnit(GeometryBuffer::GeometryBufferTextureType_Diffuse);
		m_pGamePointLightEffect->setNormalTextureUnit(GeometryBuffer::GeometryBufferTextureType_Normal);
		m_pGamePointLightEffect->setScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

		m_pGameStencilEffect = new NoneShader(NoneVSFileName.c_str(), NoneFSFileName.c_str());

		//m_pGameShadowEffect = new ShadowMapShader(pShadowVSFileName, pShadowFSFileName);
		
		//if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT))
		//{
		//	return false;
		//}

		m_pMesh = new Mesh();
		m_pPointLightSphere = new Mesh();
		m_pDirLightQuad = new Mesh();
		// 加载不同的模型文件/
		bool flag = m_pMesh->LoadMesh("Resource/crytek-sponza/sponza.obj")
			&& m_pPointLightSphere->LoadMesh("Resource/sphere.obj")
			&& m_pDirLightQuad->LoadMesh("Resource/quad.obj");

		Point3 sceneCenter = m_pMesh->GetSceneCenterPos();
		m_pCamera->setCamera(Point4(sceneCenter.x, sceneCenter.y, sceneCenter.z, 1.0f), targetVector, upVector);
		m_pCamera->setDistanceFactor(m_pMesh->getSuitableDistanceFactor());
		m_pCamera->setProjectionMatrix(75.0, 4 / 3, 1, 10000);

		return flag;
	}
	void display() override
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);

		RenderProcessing();

		/***************/
		//GeometryProcessing();
		////glEnable(GL_STENCIL_TEST);
		//for (size_t i = 0; i < m_pointLights.size(); i++)
		//{
		//	//StencilTestPass(i);
		//	PointLightProcessing(i);
		//}
		////glDisable(GL_STENCIL_TEST);
		//DirectionalLightProcessing();
		//m_geometryBufferObject.CopyDepthBuffer();
		//FinalPass();
	}
	void GeometryProcessing()
	{
		m_geometryBufferObject.BindForGeometryPass();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pGameGBufferEffect->Bind();
		// 只有G-Buffer处理阶段开启深度检测/
		//glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_pGameGBufferEffect->SetMVPMatrix(m_pCamera->getMVPMatrix());
		m_pGameGBufferEffect->SetModelMatrix(m_pCamera->getModelMatrix());
		m_pGameGBufferEffect->SetNormalMatrix(m_pCamera->getNormTransformMatrix());

		m_pMesh->Render();
		m_geometryBufferObject.UnBind();
		// G-Buffer处理完毕，关闭深度检测/
		//glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}
	void StencilTestPass(unsigned int pointLightIndex)
	{
		m_pGameStencilEffect->Bind();
		// 关闭颜色/深度缓冲的写入，开启深度测试，开启模板缓冲用于模板测试/
		m_geometryBufferObject.BindForStencilPass();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);
		// 配置模板测试策略/
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		// 渲染点光源球体/
		float scaleRadius = CalcPointLightSphere(m_pointLights[pointLightIndex]);
		//printf("pointLight: %d, radius: %f\n", pointLightIndex, scaleRadius);
		m_pCamera->setScale(Vec3(scaleRadius, scaleRadius, scaleRadius));
		m_pCamera->setWorldPos(m_pointLights[pointLightIndex].PointLightPos);

		m_pGameStencilEffect->SetMVPMatrix(m_pCamera->getMVPMatrix());

		m_pPointLightSphere->Render();
		m_pCamera->recoverModelTransform();
	}
	void PointLightProcessing(unsigned int pointLightIndex)
	{
		// temp
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glClear(GL_COLOR_BUFFER_BIT);
		// 设置点光源位置/
		static float pointLightMoveSpeed = 150.0f;
		static float pointLightHeightParam = 0.0f;
		pointLightHeightParam += pointLightMoveSpeed * m_deltaTime;	// 使得移动速度不受帧数影响/
		float lightPosParam = cosf(ToRadian(pointLightHeightParam)) * m_pMesh->getSuitableDistanceFactor();
		Point4 sceneCenterPos = Point4(m_pMesh->GetSceneCenterPos());

		static float pointLightBaseX = sceneCenterPos.x;
		m_pointLights[0].PointLightPos = Point4(pointLightBaseX + lightPosParam * 2.0f, sceneCenterPos.y * 1.2f, sceneCenterPos.z);

		// PointLightShader所需数据/
		// 光照计算放在世界坐标下/
		m_pGamePointLightEffect->Bind();
		m_geometryBufferObject.BindForLightPass();
		m_pGamePointLightEffect->setEyeWorldPos(m_pCamera->getEyePosition());
		//glStencilFunc(GL_NOTEQUAL, 0, 0xff);
		//glStencilFunc(GL_ALWAYS, 0, 0);
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		//float scaleRadius = CalcPointLightSphere(m_pointLights[pointLightIndex]);
		//printf("pointLight: %d, radius: %f\n", pointLightIndex, scaleRadius);
		//m_pCamera->setScale(Vec3(scaleRadius, scaleRadius, scaleRadius));
		m_pCamera->setWorldPos(m_pointLights[pointLightIndex].PointLightPos);

		m_pGamePointLightEffect->setPointLightsParams(m_pointLights.size(), &m_pointLights[pointLightIndex]);
		m_pGamePointLightEffect->setMVPMatrix(m_pCamera->getMVPMatrix());

		//m_pPointLightSphere->Render();
		Mesh::RenderQuad();
		m_pCamera->recoverModelTransform();
		//glCullFace(GL_BACK);
		//glDisable(GL_BLEND);
	}
	void DirectionalLightProcessing()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pGameDirLightEffect->Bind();
		m_geometryBufferObject.BindForLightPass();
		m_pGameDirLightEffect->setEyeWorldPos(m_pCamera->getEyePosition());

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		//m_pDirLightQuad->Render();
		Mesh::RenderQuad();

		glDisable(GL_BLEND);
	}
	void FinalPass()
	{
		//m_geometryBufferObject.BindForFinalPass();
		//glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		//	0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	}

	void ShadowMapProcessing()
	{
		//m_shadowMapFBO.BindForWriting();
		//glClear(GL_DEPTH_BUFFER_BIT);
		//m_pGameShadowEffect->Bind();
	}
	void RenderProcessing()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pGameLightEffect->Bind();

		static float pointLightMoveSpeed = 150.0f;
		static float pointLightHeightParam = 0.0f;
		pointLightHeightParam += pointLightMoveSpeed * m_deltaTime;	// 使得移动速度不受帧数影响/
		float lightPosParam = cosf(ToRadian(pointLightHeightParam)) * m_pMesh->getSuitableDistanceFactor();
		Point4 sceneCenterPos = Point4(m_pMesh->GetSceneCenterPos());

		static float pointLightBaseX = sceneCenterPos.x;
		Point4 pointLightPos = Point4(pointLightBaseX + lightPosParam * 2.0f, sceneCenterPos.y * 1.2f, sceneCenterPos.z);
		m_pointLights[0].PointLightPos = Point4(pointLightPos.x, pointLightPos.y, pointLightPos.z);

		static float spotLightBaseHeight = sceneCenterPos.y;
		Point4 spotLightPos = Point4(sceneCenterPos.x, spotLightBaseHeight + lightPosParam, sceneCenterPos.z);
		m_spotLights[0].PointLightPos = Point4(spotLightPos.x, spotLightPos.y, spotLightPos.z);

		// 光照计算放在世界坐标下/
		m_pGameLightEffect->setMVPMatrix(m_pCamera->getMVPMatrix());
		m_pGameLightEffect->setModelMatrix(m_pCamera->getModelMatrix());
		m_pGameLightEffect->setNormTransformMatrix(m_pCamera->getNormTransformMatrix());
		m_pGameLightEffect->setDirLightParams(m_dirLight);
		m_pGameLightEffect->setEyeWorldPos(m_pCamera->getEyePosition());
		m_pGameLightEffect->setSpecularIntensity(specularIntensity);
		m_pGameLightEffect->setSpecularPower(specularPower);
		m_pGameLightEffect->setPointLightsParams(1, &m_pointLights[0]);
		m_pGameLightEffect->setSpotLightsParams(1, &m_spotLights[0]);

		m_pMesh->Render();
	}
private:
	// 解关于衰减半径的二元一次方程/
	float CalcPointLightSphere(const PointLight& light)
	{
		float maxChannel = fmax(fmax(light.Color.x, light.Color.y), light.Color.z);
		// 此处衰减阈值定义为1/256；若想改变阈值A，则方程中参数改为maxChannel*Identity/A
		float result = (-light.AttenuationFactor.Linear) + sqrtf(light.AttenuationFactor.Linear * light.AttenuationFactor.Linear - 4 * light.AttenuationFactor.Exp * (light.AttenuationFactor.Constant - 256 * maxChannel * light.DiffuseIntensity))
			/ (2 * light.AttenuationFactor.Exp);

		return result;
	}
	void InitLights()
	{
		m_dirLight.Color = Vec3(1.0f, 1.0f, 1.0f);
		m_dirLight.AmbientIntensity = 0.15f;
		m_dirLight.Direction = Vec4(0.0f, -1.0f, 0.0f, 0.0f);	// 平行光从天空竖直射向地面/
		m_dirLight.DiffuseIntensity = 0.2f;

		m_pointLights.resize(pointLightNum);
		m_spotLights.resize(spotLightNum);
		// 光源位置持续变化，故放在display()中设置/
		m_pointLights[0].Color = Vec3(0.5, 1.0, 0.0);
		m_pointLights[0].AmbientIntensity = 0.8f;
		m_pointLights[0].DiffuseIntensity = 0.5f;
		m_pointLights[0].AttenuationFactor.Constant = 1.0f;
		m_pointLights[0].AttenuationFactor.Linear = 1e-2f;
		m_pointLights[0].AttenuationFactor.Exp = 1e-4f;

		m_spotLights[0].AmbientIntensity = 0.9f;
		m_spotLights[0].DiffuseIntensity = 0.8f;
		m_spotLights[0].Color = Vec3(0.0f, 0.0f, 1.0f);
		m_spotLights[0].Direction = Vec4(0.0, -1.0, 0.0, 0.0);
		m_spotLights[0].AttenuationFactor.Constant = 1.0f;
		m_spotLights[0].AttenuationFactor.Linear = 1e-4f;
		m_spotLights[0].AttenuationFactor.Exp = 1e-5f;
		m_spotLights[0].CutOffAngle = 30.0f;
	}
private:
	LightShader*			m_pGameLightEffect;
	DirLightShader*			m_pGameDirLightEffect;
	PointLightShader*		m_pGamePointLightEffect;
	SpotLightShader*		m_pGameSpotLightEffect;
	GeometryBufferShader*	m_pGameGBufferEffect;
	NoneShader*				m_pGameStencilEffect;
	ShadowMapShader*		m_pGameShadowEffect;
	Mesh*					m_pMesh;
	Mesh*					m_pDirLightQuad;
	Mesh*					m_pPointLightSphere;
	DirectionalLight		m_dirLight;
	std::vector<PointLight>	m_pointLights;
	std::vector<SpotLight>	m_spotLights;
	GeometryBuffer			m_geometryBufferObject;
	//ShadowMapFBO		m_shadowMapFBO;
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
