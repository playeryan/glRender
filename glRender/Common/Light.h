#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Math/Vec3.h"
#include "Math/Vec4.h"

namespace Light
{
	static const int pointLightNum = 32;
	static const int spotLightNum = 1;
	static const float AmbientIntensity = 0.2f;		// 对所有光源来说，环境光强度应相同/
	static float specularIntensity = 1.0f;
	static float specularPower = 10;
}

struct BaseLight
{
	Vec3 Color;
	float DiffuseIntensity;
};

struct DirectionalLight : public BaseLight
{
	Vec4 Direction;
};

struct PointLight : public BaseLight
{
	Point4 PointLightPos;
	struct LightAttenuationFactor
	{
		float Constant;
		float Linear;
		float Exp;
	} AttenuationFactor;
};

struct SpotLight : public PointLight
{
	SpotLight()
		: Direction(Vec4(0.0, 0.0, 0.0))
		, CutOffAngle(0.0)
	{}
	Vec4 Direction;
	float CutOffAngle;
};

#endif // !LIGHT_H

