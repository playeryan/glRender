#pragma once
#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <gl\glew.h>

class Bitmap
{
public:
	enum Format 
	{
		Format_Grayscale =			1,	// 灰度 1通道图片/
		Format_GrayscaleAlpha =		2,	// 灰度 Alpha 2通道图片/
		Format_RGB =				3,	// R,G,B 3通道图片/
		Format_RGBA =				4	// R,G,B,Alpha 4通道图片/
	};
public:
	Bitmap() = default;
	Bitmap(	unsigned width,
			unsigned height,
			Format format,
			const unsigned char* pixels = NULL	);
	~Bitmap();

	static Bitmap* bitmapFromFile(std::string filePath);

	unsigned char* getPixels();
	unsigned getBMPWidth();
	unsigned getBMPHeight();
	GLenum getMapFormat();
	std::string getMapFormatStr();


private:
	Format			m_format;
	unsigned		m_width;
	unsigned		m_height;
	unsigned char*	m_pixels;

	void set(unsigned width, unsigned height, Format format, const unsigned char* pixels);
};
#endif // !BITMAP_H

