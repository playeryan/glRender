#pragma once
#ifndef GEOMETRYBUFFEROBJECT_H
#define GEOMETRYBUFFEROBJECT_H

#include <gl/glew.h>
#include <cstdio>
#include <vector>
#include "Tools/Utils.h"
#include "Macros/glMacros.h"

#define GeometryBufferPositionTextureUnit	0
#define GeometryBufferDiffuseTextureUnit	1
#define GeometryBufferNormalTextureUnit		2

class GeometryBuffer
{
public:
	enum GeometryBufferTextureType
	{
		GeometryBufferTextureType_Position,
		GeometryBufferTextureType_Diffuse,
		GeometryBufferTextureType_Normal,
		GeometryBufferTextureType_TexCoord,
		GeometryBufferNumTextures
	};

	GeometryBuffer();
	~GeometryBuffer();

	bool Init(unsigned int windowWidth, unsigned int windowHeight);

	//void StartFrame();
	//void BindForGeometryPass();
	//void BindForStencilPass();
	//void BindForLightPass();
	//void BindForFinalPass();
	void BindForWriting();
	void BindForReading();
	void CopyDepthBuffer();

private:
	GLuint				m_fbo;
	std::vector<GLuint>	m_textures;
	GLuint				m_depthTexture;
	GLuint				m_finalTexture;
};

#endif // !GEOMETRYBUFFEROBJECT_H

