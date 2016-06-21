#include "Texture.h"
#include "Environment.h"

Texture::Texture(const std::string & FileName, GLenum TextureTarget)
	:	m_fileName(FileName.c_str())
	,	m_textureType(NoneTexture)
	,	m_textureTarget(TextureTarget)
{

}

Texture::~Texture()
{
	SAFE_DELETE_POINTER(m_textureData);
}

bool Texture::Load()
{
	m_textureData = Bitmap::bitmapFromFile(m_fileName);
	if (!m_textureData)
	{
		//printf("file: %s, load failed\n", m_fileName.c_str());
		return false;
	}

	//m_textureData->printFormatStr();
	//printf("file: %s, load success\n", m_fileName.c_str());
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, m_textureData->getMapFormat(), (GLsizei)m_textureData->getBMPWidth(), (GLsizei)m_textureData->getBMPHeight(), 0, m_textureData->getMapFormat(), GL_UNSIGNED_BYTE, m_textureData->getPixels());
	
	// 为消除摩尔纹，使用mipmap贴图/
	glGenerateMipmap(m_textureTarget);
	// Parameters
	// 优先使用各向异性过滤/
	if (Environment::getInstance()->isSupportAnisotropic())
	{
		GLint fLargest;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		//printf("anisotropic largest level: %d\n", fLargest);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)fLargest);
	}

	glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(m_textureTarget, 0);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//printf("file: %s, load ended\n", m_fileName.c_str());
	return true;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}

void Texture::UnBind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, 0);
}

void Texture::SetTextureType(const std::string type)
{
	m_textureType = type;
}
