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

//�Զ���ṹ�壬���ڱ���TGAͼ�����ݵ�ַ��ͼ���ߡ�ͼ�����
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


