#version 430 core

const int MAX_POINT_LIGHT_NUMS = 2;
const int MAX_SPOT_LIGHT_NUMS = 2;

struct BaseLight
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
	vec3 Direction;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight
{
	BaseLight Base;
	vec3 Position;
	Attenuation Atten;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float CutOffValue;
};

uniform DirectionalLight dirLight;
uniform int pointLightNums;
uniform PointLight pointLight[MAX_POINT_LIGHT_NUMS];
uniform int spotLightNums;
uniform SpotLight spotLight[MAX_SPOT_LIGHT_NUMS];
uniform sampler2D textureUnit;
uniform sampler2D shadowMapTexture;
uniform vec3 eyeWorldPos;
uniform float specularIntensity;
uniform float specularPower;

in vec4 lightSpacePos;
in vec2 fragTexCoord;
in vec3 fragNormal; 
in vec3 vPosInWorldSpace;

out vec4 fColor;

float CalcShadowFactor(vec4 LightSpacePosition)
{
	vec3 projCoords = LightSpacePosition.xyz / LightSpacePosition.w;
	vec2 UVCoords;
	UVCoords.x = 0.5 * projCoords.x + 0.5;
	UVCoords.y = 0.5 * projCoords.y + 0.5;

	float z = 0.5 * projCoords.z + 0.5;
	float depth = texture(shadowMapTexture, UVCoords).x;

	if(depth < z + 0.00001)
	{
		return 0.5;
	}
	else
	{
		return 1.0;
	}
}

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal, float ShadowFactor)
{
	// ambient color
	vec4 ambientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
	
	// diffuse color
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	float diffuseFactor = dot(Normal, -LightDirection);		
	// specualr color
	vec4 specColor = vec4(0.0, 0.0, 0.0, 0.0);
	if (diffuseFactor > 0)
	{
		diffuseColor = vec4(Light.Color * Light.DiffuseIntensity * diffuseFactor, 1.0f);

		vec3 vertexToEye = normalize(eyeWorldPos - vPosInWorldSpace);
		//vec3 reflectLight = normalize(reflect(LightDirection, Normal));		
		//float specFactor = max(0.0, dot(vertexToEye, reflectLight));
		vec3 halfVector = normalize(LightDirection + vertexToEye);
		float specFactor = max(0.0, dot(Normal, halfVector));
		specFactor = pow(specFactor, specularPower);
		specColor = vec4(Light.Color * specularIntensity * specFactor, 1.0f);
	}

	return (ambientColor + ShadowFactor * (diffuseColor + specColor));
}

vec4 CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(dirLight.Base, dirLight.Direction, Normal, 1.0);	
}

vec4 CalcPointLight(PointLight light, vec3 Normal, vec4 LightSpacePosition)
{
	vec3 LightDirection = vPosInWorldSpace - light.Position;
	float distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	//float shadowFactor = CalcShadowFactor(LightSpacePosition);
	float shadowFactor = 1.0;
	vec4 outColor = CalcLightInternal(light.Base, LightDirection, Normal, shadowFactor);
	float attenuation = light.Atten.Constant +
						light.Atten.Linear * distance +
						light.Atten.Exp * distance * distance;
	
	return outColor / attenuation;
}

vec4 CalcSpotLight(SpotLight light, vec3 Normal, vec4 LightSpacePosition)
{
	vec3 LightToPixel = normalize(vPosInWorldSpace - light.Base.Position);
	float SpotFactor = dot(LightToPixel, light.Direction);
	if(SpotFactor > light.CutOffValue)
	{
		vec4 Color = CalcPointLight(light.Base, Normal, LightSpacePosition);
		return Color * (SpotFactor - light.CutOffValue) / (1 - light.CutOffValue);
	}
	else
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
}

void main()
{
	vec3 normalizeFragNormal = normalize(fragNormal);
	vec4 totalLight = CalcDirectionalLight(normalizeFragNormal);

	for (int i = 0; i < pointLightNums; ++i)
	{
		vec4 pointLightColor = CalcPointLight(pointLight[i], normalizeFragNormal, lightSpacePos);
		totalLight += pointLightColor;
	}

	for (int i = 0; i < spotLightNums; ++i)
	{
		vec4 spotLightColor = CalcSpotLight(spotLight[i], normalizeFragNormal, lightSpacePos);
		totalLight += spotLightColor;
	}
	
	// out color
	fColor = texture2D(textureUnit, fragTexCoord.st) * totalLight;
	//fColor = texture(textureUnit, fragTexCoord.st);
}


