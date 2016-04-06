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

/**********************LightShader, inherit BaseShaderObject******************/
class DeferredShader : public BaseShaderObject
{
public:
	DeferredShader(const char* vs, const char* fs);
	~DeferredShader();

	virtual void Init();

	void setMVPMatrix(const Matrix44& mvp);
	void setLightMVPMatrix(const Matrix44& mvp);	// for shadow map
	void setModelMatrix(const Matrix44& mv);
	void setNormTransformMatrix(const Matrix44& m);
	void setPositionTextureUnit(unsigned int TextureUnit);
	void setColorTextureUnit(unsigned int TextureUnit);
	void setNormalTextureUnit(unsigned int TextureUnit);
	void setShadowMapTexUnit(unsigned int TextureUnit);
	void setEyeWorldPos(Point4 position);
	void setSpecularIntensity(float intensity);
	void setSpecularPower(float power);
	void setScreenSize(unsigned int width, unsigned int height);
private:
	GLuint m_mvpMatrixLoc;
	GLuint m_lightMVPMatrixLoc;
	GLuint m_modelMatrixLoc;
	GLuint m_normalTransformMatrixLoc;
	GLuint m_positionTextureLoc;
	GLuint m_colorTextureLoc;
	GLuint m_normalTextureLoc;
	GLuint m_shadowMapTextureLoc;
	GLuint m_eyeWorldPosLoc;
	GLuint m_specularIntensityLoc;
	GLuint m_specularPowerLoc;
	GLuint m_screenSizeLoc;
};

/**********************DirLightShader, inherit DeferredShader******************/
class DirLightShader : public DeferredShader
{
public:
	DirLightShader(const char* vs, const char* fs);
	~DirLightShader();

	virtual void Init();

	void setDirLightParams(DirectionalLight& light);

private:
	struct dirLightLoc
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_directionLightLoc;
};

/**********************PointLightShader, inherit DeferredShader******************/
class PointLightShader : public DeferredShader
{
public:
	PointLightShader(const char* vs, const char* fs);
	~PointLightShader();

	virtual void Init();
	void setPointLightsParams(unsigned int NumLights, const PointLight* pLights);
private:
	GLuint m_numsPointLightLoc;
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
};

/**********************SpotLightShader, inherit DeferredShader******************/
class SpotLightShader : public DeferredShader
{
public:
	SpotLightShader(const char* vs, const char* fs);
	~SpotLightShader();

	virtual void Init();
	void setSpotLightsParams(unsigned int NumLights, const SpotLight* pLights);
private:
	GLuint m_numsSpotLightLoc;
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

/****************GeometryBufferShader, inherit BaseShaderObject******************/
class GeometryBufferShader : public BaseShaderObject
{
public:
	GeometryBufferShader(const char* vs, const char* fs);
	~GeometryBufferShader();

	virtual void Init();
	void SetMVPMatrix(const Matrix44& mvp);
	void SetModelMatrix(const Matrix44& m);
	void SetNormalMatrix(const Matrix44& m);
	void SetColorTextureUnit(unsigned int textureUnit);

private:
	GLuint m_mvpMatrixLoc;
	GLuint m_modelMatrixLoc;
	GLuint m_normalMatrixLoc;
	GLuint m_colorTextureUnitLoc;
};

/****************NoneShader, inherit BaseShaderObject******************/
class NoneShader : public BaseShaderObject
{
public:
	NoneShader(const char* vs, const char* fs);
	~NoneShader();

	virtual void Init();

	void SetMVPMatrix(const Matrix44& mvp);

private:
	GLuint m_mvpMatrixLoc;
};

#endif // !LIGHTTECHNIQUE_H

