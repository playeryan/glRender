#pragma once
#ifndef MYTGA_H
#define MYTGA_H
#include <cstdio>

typedef unsigned char BYTE;
typedef unsigned short WORD;

struct CTGAHEADER {
	BYTE identsize;
	BYTE colorMapType;
	BYTE imageType;
	BYTE colorMapStart[2] = { 0x00, 0x00 };
	BYTE colorMapLength[2] = { 0x00, 0x00 };
	BYTE colorMapBits;
	WORD xstart;
	WORD ystart;
	WORD width;
	WORD height;
	BYTE bits;
	BYTE descriptor;
};

//自定义结构体，用于保存TGA图像数据地址、图像宽高、图像深度
class TGAFILE 
{
public:
	TGAFILE();
	~TGAFILE();

	void ReadTGAFile(const char* fileName);
public:
	BYTE *pBits;
	int width;
	int height;
	int depth;
};

#endif


