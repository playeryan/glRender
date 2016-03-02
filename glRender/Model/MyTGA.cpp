#include "MyTGA.h"

TGAFILE::TGAFILE()
{}

TGAFILE::~TGAFILE()
{
	delete (pBits);
}

void TGAFILE::ReadTGAFile(const char* fileName)
{
	FILE *pFile;
	CTGAHEADER TGAHEADER;
	unsigned long ImageSize;
	BYTE *pBits = NULL;

	//默认/读取失败值
	width = 0;
	height = 0;
	depth = 0;

	//尝试打开文件
	pFile = fopen(fileName, "rb");
	if (pFile == NULL)
		return;

	//读入文件头,TGA文件头为18个字节大小
	fread(&TGAHEADER, 18, 1, pFile);

	//得到纹理的高度、宽度、像素深度
	width = TGAHEADER.width;
	height = TGAHEADER.height;
	depth = TGAHEADER.bits / 8;

	//验证纹理是否为8位、24位或32位的TGA图像
	if (TGAHEADER.bits != 8 && TGAHEADER.bits != 24 && TGAHEADER.bits != 32)
		return;

	//计算图像数据所需存储空间
	ImageSize = TGAHEADER.width * TGAHEADER.height * depth;

	//动态分配所需内存,并验证
	pBits = new BYTE[ImageSize];
	if (pBits == NULL)
		return;

	//向内存写入图像数据,并验证是否正常
	if (fread(pBits, ImageSize, 1, pFile) != 1) 
	{
		delete(pBits);
		return;
	}

	//操作完成
	fclose(pFile);
}