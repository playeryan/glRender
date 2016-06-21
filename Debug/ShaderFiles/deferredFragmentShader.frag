#version 430 core
struct BaseLight
{
	vec3 Color;
	float DiffuseIntensity;
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

out vec4 FragColor;
in vec2 TexCoords;

const int NR_LIGHTS = 32;

uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D shadowMapTexture;
uniform int pointLightNums;
uniform PointLight pointLight[NR_LIGHTS];
uniform vec3 eyeWorldPos;
uniform vec2 screenSize;
uniform float ambientIntensity;
uniform float specularIntensity;
uniform float specularPower;
uniform int drawMode;

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

// 假设环境光分量与光源无关，仅计算漫反射与镜面反射/
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection,  vec3 WorldPos, vec3 Normal, float SpecIntensity, float ShadowFactor)
{
    // ambient color
    //vec4 ambientColor = vec4(Light.Color * ambientIntensity, 1.0f);
    
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
		//specColor = vec4(Light.Color * specularIntensity * specFactor, 1.0f);
		specColor = vec4(Light.Color * SpecIntensity * specFactor, 1.0f);
    }

    return ShadowFactor * (diffuseColor + specColor);
}

vec4 CalcPointLight(PointLight light, vec3 WorldPos, vec3 Normal, float SpecIntensity)
{
    vec3 LightDirection = WorldPos - light.Position;
    float distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    //float shadowFactor = CalcShadowFactor(LightSpacePosition);
    float shadowFactor = 1.0;
    vec4 outColor = CalcLightInternal(light.Base, LightDirection, WorldPos, Normal, SpecIntensity, shadowFactor);
	float attenuation = light.Atten.Constant +
                        light.Atten.Linear * distance +
                        light.Atten.Exp * distance * distance;
    
    return outColor / attenuation;
}

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

void main()
{             
	//vec2 TexCoords = CalcTexCoord();
    // Retrieve data from gbuffer
    vec3 FragPos = texture(PositionMap, TexCoords).xyz;
    vec3 Diffuse = texture(DiffuseMap, TexCoords).xyz;
	vec3 Normal = texture(NormalMap, TexCoords).xyz;
	Normal = normalize(Normal);
	vec3 specTex = texture(SpecularMap, TexCoords).xyz;
	float specIntensity = texture(SpecularMap, TexCoords).r;
    
	vec4 totalLight = vec4(0.0, 0.0, 0.0, 0.0);
	// 环境光与光源无关/
	vec4 ambientLight  = vec4(Diffuse * ambientIntensity, 1.0f);
	totalLight += ambientLight;
	for (int i = 0; i < pointLightNums; ++i)
	{
		vec4 pointLightColor = CalcPointLight(pointLight[i], FragPos, Normal, specIntensity);
		totalLight += pointLightColor;
	}
	
	//FragColor = vec4(Diffuse, 1.0) * totalLight;   
	if (drawMode == 1){
		FragColor = vec4(Diffuse, 1.0) * totalLight;
	}
	else if (drawMode == 2){
		FragColor = vec4(FragPos, 1.0);
	}
	else if (drawMode == 3){
		FragColor = vec4(Diffuse, 1.0);
	}
	else if (drawMode == 4){
		FragColor = vec4(Normal, 1.0);
	}
	else if (drawMode == 5){
		//FragColor = vec4(specTex, 1.0);
		FragColor = vec4(vec3(specIntensity), 1.0);
		//FragColor = vec4(Diffuse, specIntensity);
	}
}
