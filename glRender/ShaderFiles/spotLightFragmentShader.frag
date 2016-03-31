#version 430 core

uniform mat4 mvpMatrix;
 
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

out vec2 fragTexCoord;
 
void main()
{
	fragTexCoord = vTexCoord;
	gl_Position = mvpMatrix * vec4(vPosition, 1.0) ;
}