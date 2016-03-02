#pragma once
#ifndef FRAMEBUFFEROBJECT_H

#include <gl\glew.h>
#include "Macros\glMacros.h"

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool Init(unsigned int windowWidth, unsigned int windowHeight);

	void BindForWriting();
	void BindForReading(GLenum textureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};

#endif // !FRAMEBUFFEROBJECT_H

