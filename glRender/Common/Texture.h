#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include "../Model/Bitmap.h"
#include "../Macros/glMacros.h"

const std::string NoneTexture		=	"NoneTexture";
const std::string DiffuseTexture	=	"DiffuseTexture";
const std::string HeightTexture		=	"HeightTexture";

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& FileName, GLenum TextureTarget);
	~Texture();

	bool Load();

	void Bind(GLenum TextureUnit);
	void UnBind(GLenum TextureUnit);

	void SetTextureType(const std::string type);

private:
	std::string		m_fileName;
	std::string		m_textureType;
	GLenum			m_textureTarget; // texture target, such as GL_TEXTURE_2D..
	GLuint			m_textureObj;	 // texture id, create by glGenTexture(..)
	Bitmap*			m_textureData;
};

#endif // !TEXTURE_H

