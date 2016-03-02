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
		Format_Grayscale =			1,	// �Ҷ� 1ͨ��ͼƬ/
		Format_GrayscaleAlpha =		2,	// �Ҷ� Alpha 2ͨ��ͼƬ/
		Format_RGB =				3,	// R,G,B 3ͨ��ͼƬ/
		Format_RGBA =				4	// R,G,B,Alpha 4ͨ��ͼƬ/
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

