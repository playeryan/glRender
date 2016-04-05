#pragma once
#ifndef LIGHTTECHNIQUE_H
#define LIGHTTECHNIQUE_H

#include <string>
#include <GL/glew.h>
#include "Tools/LoadShaders.h"
#include "Math/Matrix44.h"
#include "Light.h"

#define MAX_POINT_LIGHT_NUMS 2
#define MAX_SPOT_LIGHT_NUMS 2

/****************************BaseShaderObject***********************************/
class BaseShaderObject
{
public:
	BaseShaderObject(const char* vs, const char* fs);
	virtual ~BaseShaderObject();

	virtual void Init() = 0;	// 子类在Init()中应获取所需uniform值的位置/
	void Bind();
protected:
	void BaseLoadShader();		// 子类应在其Init()调用BaseLoadShader()/
	GLint getUniformLocationInShader(const char* pUniformName);
private:
	GLuint m_shaderProg;
	const char* m_pVSFileName;
	const char* m_pFSFileName;
};

/**********************LightShader, inherit BaseShaderObject******************/
class LightShader : public BaseShaderObject
{
public:
	LightShader(const char* vs, const char* fs);
	~LightShader();

	virtual void Init();

	void setMVPMatrix(const Matrix44& mvp);
	void setLightMVPMatrix(const Matrix44& mvp);
	void setModelMatrix(const Matrix44& mv);
	void setNormTransformMatrix(const Matrix44& m);
	void setTextureUnit(unsigned int TextureUnit);
	void setShadowMapTexUnit(unsigned int TextureUnit);
	void setDirLightParams(DirectionalLight& light);
	void setEyeWorldPos(Point4 position);
	void setSpecularIntensity(float intensity);
	void setSpecularPower(float power);
	void setPointLightsParams(unsigned int NumLights, const PointLight* pLights);
	void setSpotLightsParams(unsigned int NumLights, const SpotLight* pLights);
private:
	GLuint m_mvpMatrixLoc;
	GLuint m_lightMVPMatrixLoc;
	GLuint m_modelMatrixLoc;
	GLuint m_normalTransformMatrixLoc;
	GLuint m_textureLoc;
	GLuint m_shadowMapTextureLoc;
	GLuint m_eyeWorldPosLoc;
	GLuint m_specularIntensityLoc;
	GLuint m_specularPowerLoc;
	GLuint m_numsPointLightLoc;
	GLuint m_numsSpotLightLoc;

	struct dirLightLoc
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_directionLightLoc;

	struct pointLightLoc
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct attenuationFactor
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightLoc[MAX_POINT_LIGHT_NUMS];

	struct spotLightLoc
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		GLuint Direction;
		GLuint CutOffValue;
		struct attenuationFactor
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightLoc[MAX_SPOT_LIGHT_NUMS];
};

/********************ShadowMapShader, inherit BaseShaderObject**********************/
class ShadowMapShader : public BaseShaderObject
{
public:
	ShadowMapShader(const char* vs, const char* fs);
	~ShadowMapShader();

	virtual void Init();
	void setMVPMatrix(const Matrix44& mvp);
	void setTextureUnit(unsigned int textureUnit);
private:
	GLuint m_mvpMatrixLoc;	// 用于光源坐标变换的uniform矩阵/
	GLuint m_shadowMapTexLoc;
};

#endif // !LIGHTTECHNIQUE_H

