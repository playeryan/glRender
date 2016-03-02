#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include "../Model/Bitmap.h"
#include "../Macros/glMacros.h"

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& FileName, GLenum TextureTarget);
	~Texture();

	bool Load();

	void Bind(GLenum TextureUnit);
private:
	std::string	m_fileName;
	GLenum		m_textureTarget;
	GLuint		m_textureObj;
	Bitmap*		m_texture;
};

#endif // !TEXTURE_H

