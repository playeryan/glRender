#include "Bitmap.h"

// 使用stb_image库来加载图片/
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "Common\ThirdParty\stb_image\stb_image.h"

Bitmap::Bitmap(	unsigned width,
				unsigned height,
				Format format,
				const unsigned char* pixels	)
	:	m_pixels(NULL)
{
	set(width, height, format, pixels);
}

Bitmap::~Bitmap()
{
	if(m_pixels)
	{
		//delete (m_pixels);
		free(m_pixels);
		m_pixels = NULL;
	}
}

Bitmap* Bitmap::bitmapFromFile(std::string filePath)
{
	printf("bit map from file: %s, start\n", filePath.c_str());
	int width, height, channels;
	printf("before stbi load file: %s\n", filePath.c_str());
	unsigned char* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	printf("after stbi load file: %s\n", filePath.c_str());
	if (!pixels)
	{
		printf("file: %s, parse pixels failed\n", filePath);
		throw std::runtime_error(stbi_failure_reason());
	}

	Bitmap* bmp = new Bitmap(width, height, (Format)channels, pixels);
	stbi_image_free(pixels);

	return bmp;
}

unsigned char * Bitmap::getPixels()
{
	return m_pixels;
}

unsigned Bitmap::getBMPWidth()
{
	return m_width;
}

unsigned Bitmap::getBMPHeight()
{
	return m_height;
}

GLenum Bitmap::getMapFormat()
{
	GLenum format = GL_ALPHA;
	switch (m_format)
	{
	case Bitmap::Format_Grayscale:
		break;
	case Bitmap::Format_GrayscaleAlpha:
		break;
	case Bitmap::Format_RGB:
		format = GL_RGB;
		break;
	case Bitmap::Format_RGBA:
		format = GL_RGBA;
		break;
	default:
		break;
	}

	return format;
}

std::string Bitmap::getMapFormatStr()
{
	std::string formatStr;
	switch (m_format)
	{
	case Bitmap::Format_Grayscale:
		formatStr = "Grayscale";
		break;
	case Bitmap::Format_GrayscaleAlpha:
		formatStr = "GrayscaleAlpha";
		break;
	case Bitmap::Format_RGB:
		formatStr = "RGB";
		break;
	case Bitmap::Format_RGBA:
		formatStr = "RGBA";
		break;
	default:
		formatStr = "Unknown";
		break;
	}

	return formatStr;
}

void Bitmap::set(unsigned width, unsigned height, Format format, const unsigned char * pixels)
{
	if (width == 0) throw std::runtime_error("Zero width bitmap");
	if (height == 0) throw std::runtime_error("Zero height bitmap");
	if (format <= 0 || format > 4) throw std::runtime_error("Invalid bitmap format");

	m_width = width;
	m_height = height;
	m_format = format;

	size_t newSize = m_width * m_height * m_format;

	if (m_pixels) {
		m_pixels = (unsigned char*)realloc(m_pixels, newSize);
	}
	else {
		m_pixels = (unsigned char*)malloc(newSize);
	}
	if (pixels)
	{
		memcpy(m_pixels, pixels, newSize);
	}
}
