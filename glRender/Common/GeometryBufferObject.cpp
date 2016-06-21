

#include "GeometryBufferObject.h"

GeometryBuffer::GeometryBuffer()
	:	m_fbo(0)
	,	m_depthTexture(0)
	,	m_finalTexture(0)
{
	m_textures.resize(GeometryBufferNumTextures);
}

GeometryBuffer::~GeometryBuffer()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
	if (m_textures[0] != 0)
	{
		glDeleteTextures(m_textures.size(), &m_textures[0]);
	}
	if (m_depthTexture != 0)
	{
		glDeleteTextures(1, &m_depthTexture);
	}
	if (m_finalTexture != 0)
	{
		glDeleteTextures(1, &m_finalTexture);
	}
}

bool GeometryBuffer::Init(unsigned int windowWidth, unsigned int windowHeight)
{
	// create frame buffer object
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	// create geometry buffer textures
	glGenTextures(m_textures.size(), &m_textures[0]);

	for (size_t i = 0; i < m_textures.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, BUFFER_OFFSET(0));
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}
	std::vector<GLenum> drawBuffers;
	for (size_t i = 0; i < m_textures.size(); i++)
	{
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

	// for depth texture
	//glGenTextures(1, &m_depthTexture);
	//glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, BUFFER_OFFSET(0));
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	//
	glGenRenderbuffers(1, &m_depthTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);

	// for final texture
	// 最终渲染结果挂载至GL_COLOR_ATTACHMENT4上
	//glGenTextures(1, &m_finalTexture);
	//glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, BUFFER_OFFSET(0));
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer error, status: 0x%x\n", status);
		return false;
	}
	// restore default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

//void GeometryBuffer::StartFrame()
//{
//	//每一帧开始时，清空上一帧的渲染结果/
//	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
//	glDrawBuffer(GL_COLOR_ATTACHMENT4);
//	glClear(GL_COLOR_BUFFER_BIT);
//}

void GeometryBuffer::BindForGeometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	//std::vector<GLenum> drawBuffers;
	//for (size_t i = 0; i < m_textures.size(); i++)
	//{
	//	drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	//}
	//glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
}

void GeometryBuffer::BindForStencilPass()
{
	// 模板测试阶段，仅向模板缓存中写入数据，禁用颜色输出/
	glDrawBuffer(GL_NONE);
}

void GeometryBuffer::BindForLightPass()
{
	//glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (size_t i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[GeometryBufferTextureType_Position + i]);
	}
}

//void GeometryBuffer::BindForFinalPass()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
//	glReadBuffer(GL_COLOR_ATTACHMENT4);
//}

void GeometryBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void GeometryBuffer::BindForWriting()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
//}
//
//void GeometryBuffer::BindForReading()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	for (size_t i = 0; i < m_textures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//		glBindTexture(GL_TEXTURE_2D, m_textures[GeometryBufferTextureType_Position + i]);
//	}
//}

void GeometryBuffer::CopyDepthBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
