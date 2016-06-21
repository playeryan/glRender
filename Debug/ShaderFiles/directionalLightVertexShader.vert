#version 430 core

uniform mat4 mvpMatrix;
 
layout(location = 0) in vec3 vPosition;
 
void main()
{
	//gl_Position = mvpMatrix * vec4(vPosition, 1.0);
	gl_Position = vec4(vPosition, 1.0);
}