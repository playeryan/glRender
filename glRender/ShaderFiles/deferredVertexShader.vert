#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;


void main()
{
	// 顶点变换已在GeometryBufferShader中完成, 并存储数据在GeometryBuffer中，作为本shader的输入/
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;
}