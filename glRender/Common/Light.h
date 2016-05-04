#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Math/Vec3.h"
#include "Math/Vec4.h"

static const int pointLightNum = 32;
static const int spotLightNum = 1;

namespace Light
{
	const float AmbientIntensity = 0.2f;
}

struct BaseLight
{
	Vec3 Color;
	//float AmbientIntensity; // 对所有光源来说，环境光强度应相同/
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

