#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;


void main()
{
	// ����任����GeometryBufferShader�����, ���洢������GeometryBuffer�У���Ϊ��shader������/
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;
}