#version 430 core

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
uniform sampler2D PositionMap;
uniform sampler2D ColorMap;
uniform sampler2D NormalMap;
uniform sampler2D shadowMapTexture;
uniform vec3 eyeWorldPos;
uniform float specularIntensity;
uniform float specularPower;
uniform vec2 screenSize;

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

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 WorldPos, vec3 Normal, float ShadowFactor)
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

		vec3 vertexToEye = normalize(eyeWorldPos - WorldPos);
		//vec3 reflectLight = normalize(reflect(LightDirection, Normal));		
		//float specFactor = max(0.0, dot(vertexToEye, reflectLight));
		vec3 halfVector = normalize(LightDirection + vertexToEye);
		float specFactor = max(0.0, dot(Normal, halfVector));
		specFactor = pow(specFactor, specularPower);
		specColor = vec4(Light.Color * specularIntensity * specFactor, 1.0f);
	}

	return (ambientColor + ShadowFactor * (diffuseColor + specColor));
}

vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
	return CalcLightInternal(dirLight.Base, dirLight.Direction, WorldPos, Normal, 1.0);	
}

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
	//vec2 result = vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y);
	//return result;
}

void main()
{
	vec2 TexCoord = CalcTexCoord();
	vec3 WorldPos = texture(PositionMap, TexCoord).xyz;
	vec3 Color = texture(ColorMap, TexCoord).xyz;
	vec3 Normal = texture(NormalMap, TexCoord).xyz;
	Normal = normalize(Normal);

	fColor = vec4(Color, 1.0) * CalcDirectionalLight(WorldPos, Normal);
}


