#version 430 core

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D HeightMap;
uniform sampler2D OpacityMap;
uniform bool hasOpacityMap;

in vec3 fragWorldPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec4 SpecularOut;
 
void main()
{
	WorldPosOut = fragWorldPosition;

	float opacity;
	if (hasOpacityMap){
		opacity = texture(OpacityMap, fragTexCoord).x;
	}
	else{
		opacity = 1.0f;
	}
	//DiffuseOut = vec4(texture(DiffuseMap, fragTexCoord).xyz, opacity);
	DiffuseOut = vec4(texture(DiffuseMap, fragTexCoord).xyz, 1.0f);
	SpecularOut = vec4(texture(SpecularMap, fragTexCoord).xyz, 1.0f);
	NormalOut = normalize(fragNormal);
}