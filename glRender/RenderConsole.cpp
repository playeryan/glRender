#include <iostream>
#include <cstdio>
#include <ctime>
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
#include "Model\Model.h"
#include "ShaderFiles\ShaderFiles.h"

using namespace std; 
using namespace Light;

Vec4	targetVector		=	Vec4(0, 0, -1, 0);
Vec4	upVector			=	Vec4(0, 1, 0, 0);

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
		,	m_pModel(NULL)
	{}
	~GLrender()
	{
		SAFE_DELETE_POINTER(m_pGameLightEffect);
		SAFE_DELETE_POINTER(m_pGameDirLightEffect);
		SAFE_DELETE_POINTER(m_pGamePointLightEffect);
		SAFE_DELETE_POINTER(m_pGameSpotLightEffect);
		SAFE_DELETE_POINTER(m_pGameGBufferEffect);
		SAFE_DELETE_POINTER(m_pGameStencilEffect);
		SAFE_DELETE_POINTER(m_pGameShadowEffect);
		SAFE_DELETE_POINTER(m_pModel);
	}

	bool init() override
	{
		if (!m_geometryBufferObject.Init(WINDOW_WIDTH, WINDOW_HEIGHT))
		{
			return false;
		}

		m_pGameGBufferEffect = new GeometryBufferShader(GBufferVSFileName.c_str(), GBufferFSFileName.c_str());
		m_pGameGBufferEffect->Bind();
		m_pGameGBufferEffect->SetDiffuseTextureUnit(TextureUnitIndex::DiffuseIndex);
		m_pGameGBufferEffect->SetSpecularTextureUnit(TextureUnitIndex::SpecularIndex);
		m_pGameGBufferEffect->SetHeightTextureUnit(TextureUnitIndex::HeightIndex);
		m_pGameGBufferEffect->SetOpacityTextureUnit(TextureUnitIndex::OpacityIndex);

		//m_pGameDirLightEffect = new DirLightShader(DirLightVSFileName.c_str(), DirLightFSFileName.c_str());
		//m_pGameDirLightEffect->Bind();
		//m_pGameDirLightEffect->setSpecularIntensity(specularIntensity);
		//m_pGameDirLightEffect->setSpecularPower(specularPower);
		//m_pGameDirLightEffect->setPositionTextureUnit(GeometryBuffer::GeometryBufferTextureType_Position);
		//m_pGameDirLightEffect->setColorTextureUnit(GeometryBuffer::GeometryBufferTextureType_Diffuse);
		//m_pGameDirLightEffect->setNormalTextureUnit(GeometryBuffer::GeometryBufferTextureType_Normal);
		//m_pGameDirLightEffect->setDirLightParams(m_dirLight);
		//m_pGameDirLightEffect->setScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		//Matrix44 m;
		//m.LoadIdentity();
		//m_pGameDirLightEffect->setMVPMatrix(m);

		//m_pGamePointLightEffect = new PointLightShader(PointLightVSFileName.c_str(), PointLightFSFileName.c_str());
		m_pGamePointLightEffect = new PointLightShader(DeferredVSFileName.c_str(), DeferredFSFileName.c_str());
		m_pGamePointLightEffect->Bind();
		m_pGamePointLightEffect->setSpecularIntensity(specularIntensity);
		m_pGamePointLightEffect->setSpecularPower(specularPower);
		m_pGamePointLightEffect->setPositionTextureUnit(GeometryBuffer::GeometryBufferTextureType_Position);
		m_pGamePointLightEffect->setDiffuseTextureUnit(GeometryBuffer::GeometryBufferTextureType_Diffuse);
		m_pGamePointLightEffect->setSpecularTextureUnit(GeometryBuffer::GeometryBufferTextureType_Specular);
		m_pGamePointLightEffect->setNormalTextureUnit(GeometryBuffer::GeometryBufferTextureType_Normal);
		m_pGamePointLightEffect->setScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_pGamePointLightEffect->setAmbientIntensity(Light::AmbientIntensity);

		//m_pGameStencilEffect = new NoneShader(NoneVSFileName.c_str(), NoneFSFileName.c_str());

		//m_pGameShadowEffect = new ShadowMapShader(pShadowVSFileName, pShadowFSFileName);
		
		//if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT))
		//{
		//	return false;
		//}

		m_pModel = new Model();
		m_pPointLightSphere = new Model();
		m_pDirLightQuad = new Model();
		// 加载不同的模型文件/
		bool flag = m_pModel->LoadModel("Resource/crytek-sponza/sponza.obj")
			&& m_pPointLightSphere->LoadModel("Resource/sphere.obj")
			&& m_pDirLightQuad->LoadModel("Resource/quad.obj");

		Point3 sceneCenter = m_pModel->GetSceneCenterPos();
		m_pCamera->setCamera(Point4(sceneCenter.x, sceneCenter.y, sceneCenter.z, 1.0f), targetVector, upVector);
		m_pCamera->setDistanceFactor(m_pModel->getSuitableDistanceFactor());
		m_pCamera->setProjectionMatrix(70.0, WINDOW_WIDTH, WINDOW_HEIGHT, 1, 10000);

		InitLights();

		return flag;
	}
	void display() override
	{
		glEnable(GL_DEPTH_TEST);
		GeometryProcessing();
		//glEnable(GL_STENCIL_TEST);

		PointLightProcessing();
		//glDisable(GL_STENCIL_TEST);
		//DirectionalLightProcessing();
		m_geometryBufferObject.CopyDepthBuffer();
		FinalPass();
	}
	void GeometryProcessing()
	{
		m_geometryBufferObject.BindForGeometryPass();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pGameGBufferEffect->Bind();
		// 只有G-Buffer处理阶段开启深度检测/
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);

		m_pGameGBufferEffect->SetMVPMatrix(m_pCamera->getMVPMatrix());
		m_pGameGBufferEffect->SetModelMatrix(m_pCamera->getModelMatrix());
		m_pGameGBufferEffect->SetNormalMatrix(m_pCamera->getNormTransformMatrix());

		m_pModel->Render(m_pGameGBufferEffect);
		m_geometryBufferObject.UnBind();
		// G-Buffer处理完毕，关闭深度检测/
		//glDepthMask(GL_FALSE);
		//glDisable(GL_DEPTH_TEST);
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

		//m_pPointLightSphere->Render();
		m_pCamera->recoverModelTransform();
	}
	void PointLightProcessing()
	{
		// temp
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_ONE, GL_ONE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pGamePointLightEffect->Bind();
		m_pGamePointLightEffect->setDrawMode(m_drawMode);
		//printf("current draw mode: %d\n", m_drawMode);
		// 设置点光源位置/
		static float pointLightMoveSpeed = 160.0f;
		static float pointLightMoveParam = 0.0f;
		pointLightMoveParam += pointLightMoveSpeed * m_deltaTime;	// 使得移动速度不受帧数影响/
		float lightPosParam = cosf(ToRadian(pointLightMoveParam)) * pointLightMoveSpeed;

		for (size_t i = 0; i < pointLightNum; i++)
		{
			m_pointLights[i].PointLightPos = m_pointLightsOriginPos[i];
			int remainder = i % 3;
			switch (remainder)
			{
				case 0:
					m_pointLights[i].PointLightPos.x += lightPosParam;
					break;
				case 1:
					m_pointLights[i].PointLightPos.y += lightPosParam;
					break;
				case 2:
					m_pointLights[i].PointLightPos.z += lightPosParam;
					break;
				default:
					break;
			}
		}

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
		/****************for light sphere***************/
		//float scaleRadius = CalcPointLightSphere(m_pointLights[pointLightIndex]);
		//printf("pointLight: %d, radius: %f\n", pointLightIndex, scaleRadius);
		//m_pCamera->setScale(Vec3(scaleRadius, scaleRadius, scaleRadius));
		//m_pCamera->setWorldPos(m_pointLights[pointLightIndex].PointLightPos);

		m_pGamePointLightEffect->setPointLightsParams(m_pointLights.size(), &m_pointLights[0]);

		//m_pPointLightSphere->Render();
		Model::RenderQuad();
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
		Model::RenderQuad();

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
		m_dirLight.Direction = Vec4(0.0f, -1.0f, 0.0f, 0.0f);	// 平行光从天空竖直射向地面/
		m_dirLight.DiffuseIntensity = 0.2f;

		m_pointLights.resize(pointLightNum);
		m_pointLightsOriginPos.resize(pointLightNum);
		m_spotLights.resize(spotLightNum);
		// 光源位置持续变化，故放在display()中设置/
		// 先记录随机得到的光源初始位置/
		unsigned int seed = (unsigned int)time(NULL);
		for (size_t i = 0; i < pointLightNum; i++)
		{
			srand(seed + i * 100);
			float randValueR = (rand() % 100) / 100.0f;
			float randValueG = (rand() % 100) / 100.0f;
			float randValueB = (rand() % 100) / 100.0f;
			float randValueD = (rand() % 30) / 100.0f + 0.05f;
			m_pointLights[i].Color = Vec3(randValueR, randValueG, randValueB);
			m_pointLights[i].DiffuseIntensity = randValueD;
			m_pointLights[i].AttenuationFactor.Constant = 1.0f;
			m_pointLights[i].AttenuationFactor.Linear = 1e-5f;
			m_pointLights[i].AttenuationFactor.Exp = 1e-5f;
		}
		for (size_t i = 0; i < pointLightNum; i++)
		{
			srand(seed + i * 100);

			float randValueX = -rand() % 1200 + 600;
			float randValueY = -rand() % 600 + 300;
			float randValueZ = -rand() % 400 + 200;
			Point4 sceneCenterPos = Point4(m_pModel->GetSceneCenterPos());
			m_pointLightsOriginPos[i] = Point4(	sceneCenterPos.x + randValueX,
												sceneCenterPos.y + randValueY,
												sceneCenterPos.z + randValueZ	);
		}

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
	Model*					m_pModel;
	Model*					m_pDirLightQuad;
	Model*					m_pPointLightSphere;
	DirectionalLight		m_dirLight;
	std::vector<PointLight>	m_pointLights;
	std::vector<Point4>		m_pointLightsOriginPos;
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
