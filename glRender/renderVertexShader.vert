#version 430 core

uniform mat4 mvpMatrix;
uniform mat4 lightMVPMatrix;
uniform mat4 modelTransformMatrix;
uniform mat4 normalTransformMatrix;
 
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

out vec4 lightSpacePos;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 vPosInWorldSpace;
 
void main()
{
	fragTexCoord = vTexCoord;
	fragNormal = (normalTransformMatrix * vec4(vNormal, 0.0)).xyz;
	vPosInWorldSpace = (modelTransformMatrix * vec4(vPosition, 1.0)).xyz;
	lightSpacePos = lightMVPMatrix * vec4(vPosition, 1.0);
	gl_Position = mvpMatrix * vec4(vPosition, 1.0) ;
}