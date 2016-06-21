#version 430 core

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
 
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

out vec3 fragWorldPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;
 
void main()
{
	fragWorldPosition = (modelMatrix * vec4(vPosition, 1.0)).xyz;
	fragTexCoord = vTexCoord;
	fragNormal = (normalMatrix * vec4(vNormal, 0.0)).xyz;
	gl_Position = mvpMatrix * vec4(vPosition, 1.0);
}