#include "ShaderObject.h"

BaseShaderObject::BaseShaderObject(const char* vs, const char* fs)
	:	m_pVSFileName(vs)
	,	m_pFSFileName(fs)
{}

BaseShaderObject::~BaseShaderObject()
{
}

void BaseShaderObject::Bind()
{
	glUseProgram(m_shaderProg);
}

void BaseShaderObject::BaseLoadShader()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, m_pVSFileName },
		{ GL_FRAGMENT_SHADER, m_pFSFileName },
		{ GL_NONE, NULL }
	};

	m_shaderProg = LoadShaders(shaders);
}

GLint BaseShaderObject::getUniformLocationInShader(const char * pUniformName)
{
	GLint loc = glGetUniformLocation(m_shaderProg, pUniformName);
	printf("%s location is : %d\n", pUniformName, loc);
	return loc;
}

LightShader::LightShader(const char* vs, const char* fs)
	: BaseShaderObject(vs, fs)
{
	Init();
}

LightShader::~LightShader()
{}

void LightShader::Init()
{
	BaseShaderObject::BaseLoadShader();

	m_mvpMatrixLoc = getUniformLocationInShader("mvpMatrix");
	m_lightMVPMatrixLoc = getUniformLocationInShader("lightMVPMatrix");
	m_modelMatrixLoc = getUniformLocationInShader("modelTransformMatrix");
	m_normalTransformMatrixLoc = getUniformLocationInShader("normalTransformMatrix");
	m_textureLoc = getUniformLocationInShader("textureUnit");
	m_shadowMapTextureLoc = getUniformLocationInShader("shadowMapTexture");
	m_directionLightLoc.Color = getUniformLocationInShader("dirLight.Base.Color");
	//m_directionLightLoc.AmbientIntensity = getUniformLocationInShader("dirLight.Base.AmbientIntensity");
	m_directionLightLoc.Direction = getUniformLocationInShader("dirLight.Direction");
	m_directionLightLoc.DiffuseIntensity = getUniformLocationInShader("dirLight.Base.DiffuseIntensity");
	m_eyeWorldPosLoc = getUniformLocationInShader("eyeWorldPos");
	m_specularIntensityLoc = getUniformLocationInShader("specularIntensity");
	m_specularPowerLoc = getUniformLocationInShader("specularPower");
	m_numsPointLightLoc = getUniformLocationInShader("pointLightNums");
	m_numsSpotLightLoc = getUniformLocationInShader("spotLightNums");

	for (int i = 0; i < pointLightNum; ++i)
	{
		char name[128] = { 0 };
		_snprintf_s(name, sizeof(name), "pointLight[%d].Base.Color", i);
		m_pointLightLoc[i].Color = getUniformLocationInShader(name);
	
		//_snprintf_s(name, sizeof(name), "pointLight[%d].Base.AmbientIntensity", i);
		//m_pointLightLoc[i].AmbientIntensity = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "pointLight[%d].Base.DiffuseIntensity", i);
		m_pointLightLoc[i].DiffuseIntensity = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "pointLight[%d].Position", i);
		m_pointLightLoc[i].Position = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "pointLight[%d].Atten.Constant", i);
		m_pointLightLoc[i].Atten.Constant = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "pointLight[%d].Atten.Linear", i);
		m_pointLightLoc[i].Atten.Linear = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "pointLight[%d].Atten.Exp", i);
		m_pointLightLoc[i].Atten.Exp = getUniformLocationInShader(name);
	}

	for (int i = 0; i < spotLightNum; ++i)
	{
		char name[128] = { 0 };
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Base.Color", i);
		m_spotLightLoc[i].Color = getUniformLocationInShader(name);
		
		//_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Base.AmbientIntensity", i);
		//m_spotLightLoc[i].AmbientIntensity = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Base.DiffuseIntensity", i);
		m_spotLightLoc[i].DiffuseIntensity = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Position", i);
		m_spotLightLoc[i].Position = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].Direction", i);
		m_spotLightLoc[i].Direction = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].CutOffValue", i);
		m_spotLightLoc[i].CutOffValue = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Atten.Constant", i);
		m_spotLightLoc[i].Atten.Constant = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Atten.Linear", i);
		m_spotLightLoc[i].Atten.Linear = getUniformLocationInShader(name);
		
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Atten.Exp", i);
		m_spotLightLoc[i].Atten.Exp = getUniformLocationInShader(name);
	}
}

void LightShader::setMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_mvpMatrixLoc, 1, true, mvp.m);
}

void LightShader::setLightMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_lightMVPMatrixLoc, 1, true, mvp.m);
}

void LightShader::setModelMatrix(const Matrix44 & mv)
{
	glUniformMatrix4fv(m_modelMatrixLoc, 1, true, mv.m);
}

void LightShader::setNormTransformMatrix(const Matrix44 & m)
{
	glUniformMatrix4fv(m_normalTransformMatrixLoc, 1, true, m.m);
}

void LightShader::setTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_textureLoc, TextureUnit);
}

void LightShader::setShadowMapTexUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapTextureLoc, TextureUnit);
}

void LightShader::setDirLightParams(DirectionalLight & light)
{
	glUniform3f(m_directionLightLoc.Color, light.Color.x, light.Color.y, light.Color.z);
	//glUniform1f(m_directionLightLoc.AmbientIntensity, light.AmbientIntensity);
	// 需提前将法线化为单位向量/
	Vec4 direction = light.Direction;
	direction.norm();
	glUniform3f(m_directionLightLoc.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_directionLightLoc.DiffuseIntensity, light.DiffuseIntensity);
}

void LightShader::setEyeWorldPos(Point4 position)
{
	glUniform3f(m_eyeWorldPosLoc, position.x, position.y, position.z);
}

void LightShader::setSpecularIntensity(float intensity)
{
	glUniform1f(m_specularIntensityLoc, intensity);
}

void LightShader::setSpecularPower(float power)
{
	glUniform1f(m_specularPowerLoc, power);
}

void LightShader::setPointLightsParams(unsigned int numLights, const PointLight* pLights)
{
	glUniform1i(m_numsPointLightLoc, numLights);

	for (size_t i = 0; i < numLights; ++i)
	{
		glUniform3f(m_pointLightLoc[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		//glUniform1f(m_pointLightLoc[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightLoc[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightLoc[i].Position, pLights[i].PointLightPos.x, pLights[i].PointLightPos.y, pLights[i].PointLightPos.z);
		glUniform1f(m_pointLightLoc[i].Atten.Constant, pLights[i].AttenuationFactor.Constant);
		glUniform1f(m_pointLightLoc[i].Atten.Linear, pLights[i].AttenuationFactor.Linear);
		glUniform1f(m_pointLightLoc[i].Atten.Exp, pLights[i].AttenuationFactor.Exp);
	}
}

void LightShader::setSpotLightsParams(unsigned int NumLights, const SpotLight * pLights)
{
	glUniform1i(m_numsSpotLightLoc, NumLights);

	for (size_t i = 0; i < NumLights; i++)
	{
		glUniform3f(m_spotLightLoc[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		//glUniform1f(m_spotLightLoc[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_spotLightLoc[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_spotLightLoc[i].Position, pLights[i].PointLightPos.x, pLights[i].PointLightPos.y, pLights[i].PointLightPos.z);
		
		Vec4 direction = pLights[i].Direction;
		direction.norm();
		glUniform3f(m_spotLightLoc[i].Direction, direction.x, direction.y, direction.z);
		glUniform1f(m_spotLightLoc[i].CutOffValue, cosf(ToRadian(pLights[i].CutOffAngle)));
		glUniform1f(m_spotLightLoc[i].Atten.Constant, pLights[i].AttenuationFactor.Constant);
		glUniform1f(m_spotLightLoc[i].Atten.Linear, pLights[i].AttenuationFactor.Linear);
		glUniform1f(m_spotLightLoc[i].Atten.Exp, pLights[i].AttenuationFactor.Exp);
	}
}

/**********************DeferredShader, inherit BaseShaderObject******************/
DeferredShader::DeferredShader(const char* vs, const char* fs)
	: BaseShaderObject(vs, fs)
{
	Init();
}

DeferredShader::~DeferredShader()
{}

void DeferredShader::Init()
{
	BaseShaderObject::BaseLoadShader();

	m_mvpMatrixLoc = getUniformLocationInShader("mvpMatrix");
	m_lightMVPMatrixLoc = getUniformLocationInShader("lightMVPMatrix");
	m_positionTextureLoc = getUniformLocationInShader("PositionMap");
	m_colorTextureLoc = getUniformLocationInShader("ColorMap");
	m_normalTextureLoc = getUniformLocationInShader("NormalMap");
	m_shadowMapTextureLoc = getUniformLocationInShader("shadowMapTexture");
	m_eyeWorldPosLoc = getUniformLocationInShader("eyeWorldPos");
	m_specularIntensityLoc = getUniformLocationInShader("specularIntensity");
	m_specularPowerLoc = getUniformLocationInShader("specularPower");
	m_screenSizeLoc = getUniformLocationInShader("screenSize");
	m_ambientIntensity = getUniformLocationInShader("ambientIntensity");
}

void DeferredShader::setMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_mvpMatrixLoc, 1, true, mvp.m);
}

void DeferredShader::setLightMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_lightMVPMatrixLoc, 1, true, mvp.m);
}

void DeferredShader::setPositionTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_positionTextureLoc, TextureUnit);
}

void DeferredShader::setColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorTextureLoc, TextureUnit);
}

void DeferredShader::setNormalTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_normalTextureLoc, TextureUnit);
}

void DeferredShader::setShadowMapTexUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapTextureLoc, TextureUnit);
}

void DeferredShader::setEyeWorldPos(Point4 position)
{
	glUniform3f(m_eyeWorldPosLoc, position.x, position.y, position.z);
}

void DeferredShader::setSpecularIntensity(float intensity)
{
	glUniform1f(m_specularIntensityLoc, intensity);
}

void DeferredShader::setSpecularPower(float power)
{
	glUniform1f(m_specularPowerLoc, power);
}

void DeferredShader::setScreenSize(unsigned int width, unsigned int height)
{
	glUniform2f(m_screenSizeLoc, (float)width, (float)height);
}

void DeferredShader::setAmbientIntensity(float intensity)
{
	glUniform1f(m_ambientIntensity, intensity);
}

/**********************DirLightShader, inherit LightShader******************/
DirLightShader::DirLightShader(const char* vs, const char* fs)
	: DeferredShader(vs, fs)
{
	Init();
}

DirLightShader::~DirLightShader()
{
}

void DirLightShader::Init()
{
	m_directionLightLoc.Color = getUniformLocationInShader("dirLight.Base.Color");
	//m_directionLightLoc.AmbientIntensity = getUniformLocationInShader("dirLight.Base.AmbientIntensity");
	m_directionLightLoc.Direction = getUniformLocationInShader("dirLight.Direction");
	m_directionLightLoc.DiffuseIntensity = getUniformLocationInShader("dirLight.Base.DiffuseIntensity");
}

void DirLightShader::setDirLightParams(DirectionalLight & light)
{
	glUniform3f(m_directionLightLoc.Color, light.Color.x, light.Color.y, light.Color.z);
	//glUniform1f(m_directionLightLoc.AmbientIntensity, light.AmbientIntensity);
	// 需提前将法线化为单位向量/
	Vec4 direction = light.Direction;
	direction.norm();
	glUniform3f(m_directionLightLoc.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_directionLightLoc.DiffuseIntensity, light.DiffuseIntensity);
}

/**********************PointLightShader, inherit LightShader******************/
PointLightShader::PointLightShader(const char* vs, const char* fs)
	: DeferredShader(vs, fs)
{
	Init();
}

PointLightShader::~PointLightShader()
{}

void PointLightShader::Init()
{
	m_numsPointLightLoc = getUniformLocationInShader("pointLightNums");
	for (int i = 0; i < pointLightNum; ++i)
	{
		char name[128] = { 0 };
		_snprintf_s(name, sizeof(name), "pointLight[%d].Base.Color", i);
		m_pointLightLoc[i].Color = getUniformLocationInShader(name);

		//_snprintf_s(name, sizeof(name), "pointLight[%d].Base.AmbientIntensity", i);
		//m_pointLightLoc[i].AmbientIntensity = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "pointLight[%d].Base.DiffuseIntensity", i);
		m_pointLightLoc[i].DiffuseIntensity = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "pointLight[%d].Position", i);
		m_pointLightLoc[i].Position = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "pointLight[%d].Atten.Constant", i);
		m_pointLightLoc[i].Atten.Constant = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "pointLight[%d].Atten.Linear", i);
		m_pointLightLoc[i].Atten.Linear = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "pointLight[%d].Atten.Exp", i);
		m_pointLightLoc[i].Atten.Exp = getUniformLocationInShader(name);
	}
}

void PointLightShader::setPointLightsParams(unsigned int numLights, const PointLight* pLights)
{
	glUniform1i(m_numsPointLightLoc, numLights);

	for (size_t i = 0; i < numLights; ++i)
	{
		glUniform3f(m_pointLightLoc[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		//glUniform1f(m_pointLightLoc[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightLoc[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightLoc[i].Position, pLights[i].PointLightPos.x, pLights[i].PointLightPos.y, pLights[i].PointLightPos.z);
		glUniform1f(m_pointLightLoc[i].Atten.Constant, pLights[i].AttenuationFactor.Constant);
		glUniform1f(m_pointLightLoc[i].Atten.Linear, pLights[i].AttenuationFactor.Linear);
		glUniform1f(m_pointLightLoc[i].Atten.Exp, pLights[i].AttenuationFactor.Exp);
	}
}

/**********************SpotLightShader, inherit LightShader******************/
SpotLightShader::SpotLightShader(const char* vs, const char* fs)
	: DeferredShader(vs, fs)
{
	Init();
}

SpotLightShader::~SpotLightShader()
{}

void SpotLightShader::Init()
{
	m_numsSpotLightLoc = getUniformLocationInShader("spotLightNums");

	for (int i = 0; i < spotLightNum; ++i)
	{
		char name[128] = { 0 };
		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Base.Color", i);
		m_spotLightLoc[i].Color = getUniformLocationInShader(name);

		//_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Base.AmbientIntensity", i);
		//m_spotLightLoc[i].AmbientIntensity = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Base.DiffuseIntensity", i);
		m_spotLightLoc[i].DiffuseIntensity = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Position", i);
		m_spotLightLoc[i].Position = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].Direction", i);
		m_spotLightLoc[i].Direction = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].CutOffValue", i);
		m_spotLightLoc[i].CutOffValue = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Atten.Constant", i);
		m_spotLightLoc[i].Atten.Constant = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Atten.Linear", i);
		m_spotLightLoc[i].Atten.Linear = getUniformLocationInShader(name);

		_snprintf_s(name, sizeof(name), "spotLight[%d].Base.Atten.Exp", i);
		m_spotLightLoc[i].Atten.Exp = getUniformLocationInShader(name);
	}
}

void SpotLightShader::setSpotLightsParams(unsigned int NumLights, const SpotLight * pLights)
{
	glUniform1i(m_numsSpotLightLoc, NumLights);

	for (size_t i = 0; i < NumLights; i++)
	{
		glUniform3f(m_spotLightLoc[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		//glUniform1f(m_spotLightLoc[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_spotLightLoc[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_spotLightLoc[i].Position, pLights[i].PointLightPos.x, pLights[i].PointLightPos.y, pLights[i].PointLightPos.z);

		Vec4 direction = pLights[i].Direction;
		direction.norm();
		glUniform3f(m_spotLightLoc[i].Direction, direction.x, direction.y, direction.z);
		glUniform1f(m_spotLightLoc[i].CutOffValue, cosf(ToRadian(pLights[i].CutOffAngle)));
		glUniform1f(m_spotLightLoc[i].Atten.Constant, pLights[i].AttenuationFactor.Constant);
		glUniform1f(m_spotLightLoc[i].Atten.Linear, pLights[i].AttenuationFactor.Linear);
		glUniform1f(m_spotLightLoc[i].Atten.Exp, pLights[i].AttenuationFactor.Exp);
	}
}

/********************ShadowMapShader, inherit BaseShaderObject**********************/
ShadowMapShader::ShadowMapShader(const char* vs, const char* fs)
	:	BaseShaderObject(vs, fs)
{
	Init();
}

ShadowMapShader::~ShadowMapShader()
{}

void ShadowMapShader::Init()
{
	BaseShaderObject::BaseLoadShader();
	m_mvpMatrixLoc = getUniformLocationInShader("mvpMatrix");
	m_shadowMapTexLoc = getUniformLocationInShader("shadowMapTexture");
}

void ShadowMapShader::setMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_mvpMatrixLoc, 1, true, mvp.m);
}

void ShadowMapShader::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_shadowMapTexLoc, textureUnit);
}

/****************GeometryBufferShader, inherit BaseShaderObject******************/
GeometryBufferShader::GeometryBufferShader(const char* vs, const char* fs)
	: BaseShaderObject(vs, fs)
{
	Init();
}

GeometryBufferShader::~GeometryBufferShader()
{}

void GeometryBufferShader::Init()
{
	BaseShaderObject::BaseLoadShader();
	m_mvpMatrixLoc = getUniformLocationInShader("mvpMatrix");
	m_modelMatrixLoc = getUniformLocationInShader("modelMatrix");
	m_normalMatrixLoc = getUniformLocationInShader("normalMatrix");
	m_colorTextureUnitLoc = getUniformLocationInShader("ColorMap");
	m_heightTextureUnitLoc = getUniformLocationInShader("HeightMap");
}

void GeometryBufferShader::SetMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_mvpMatrixLoc, 1, true, mvp.m);
}

void GeometryBufferShader::SetModelMatrix(const Matrix44 & m)
{
	glUniformMatrix4fv(m_modelMatrixLoc, 1, true, m.m);
}

void GeometryBufferShader::SetNormalMatrix(const Matrix44 & m)
{
	glUniformMatrix4fv(m_normalMatrixLoc, 1, true, m.m);
}

void GeometryBufferShader::SetColorTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_colorTextureUnitLoc, textureUnit);
}

void GeometryBufferShader::SetHeightTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_heightTextureUnitLoc, textureUnit);
}

/****************NoneShader, inherit BaseShaderObject******************/
NoneShader::NoneShader(const char* vs, const char* fs)
	: BaseShaderObject(vs, fs)
{
	Init();
}

NoneShader::~NoneShader()
{
}

void NoneShader::Init()
{
	BaseShaderObject::BaseLoadShader();
	m_mvpMatrixLoc = getUniformLocationInShader("mvpMatrix");
}

void NoneShader::SetMVPMatrix(const Matrix44 & mvp)
{
	glUniformMatrix4fv(m_mvpMatrixLoc, 1, true, mvp.m);
}
