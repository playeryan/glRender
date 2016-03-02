#pragma once
#ifndef LIGHTTECHNIQUE_H
#define LIGHTTECHNIQUE_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include "Tools/LoadShaders.h"
#include "Math/Matrix44.h"
#include "Light.h"

#define MAX_POINT_LIGHT_NUMS 2
#define MAX_SPOT_LIGHT_NUMS 2

class LightTechnique
{
public:
	LightTechnique();
	~LightTechnique();

	void loadShader();

	GLint getUniformLocationInShader(const char* pUniformName);

	GLint getProgramParam(GLint param);

	void setMVPMatrix(const Matrix44& mvp);
	void setModelMatrix(const Matrix44& mv);
	void setNormTransformMatrix(const Matrix44& m);
	void setTextureSampler(unsigned int TextureUnit);
	void setDirLightParams(DirectionalLight& light);
	void setEyeWorldPos(Point4 position);
	void setSpecularIntensity(float intensity);
	void setSpecularPower(float power);
	void setPointLightsParams(unsigned int NumLights, const PointLight* pLights);
	void setSpotLightsParams(unsigned int NumLights, const SpotLight* pLights);

private:
	GLuint m_shaderProg;
	GLuint m_mvpMatrixLoc;
	GLuint m_modelMatrixLoc;
	GLuint m_normalTransformMatrixLoc;
	GLuint m_texSamplerLoc;
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

	static char* m_pVSFileName;
	static char* m_pFSFileName;
};


#endif // !LIGHTTECHNIQUE_H

