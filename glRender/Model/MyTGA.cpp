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

	//Ĭ��/��ȡʧ��ֵ
	width = 0;
	height = 0;
	depth = 0;

	//���Դ��ļ�
	pFile = fopen(fileName, "rb");
	if (pFile == NULL)
		return;

	//�����ļ�ͷ,TGA�ļ�ͷΪ18���ֽڴ�С
	fread(&TGAHEADER, 18, 1, pFile);

	//�õ�����ĸ߶ȡ���ȡ��������
	width = TGAHEADER.width;
	height = TGAHEADER.height;
	depth = TGAHEADER.bits / 8;

	//��֤�����Ƿ�Ϊ8λ��24λ��32λ��TGAͼ��
	if (TGAHEADER.bits != 8 && TGAHEADER.bits != 24 && TGAHEADER.bits != 32)
		return;

	//����ͼ����������洢�ռ�
	ImageSize = TGAHEADER.width * TGAHEADER.height * depth;

	//��̬���������ڴ�,����֤
	pBits = new BYTE[ImageSize];
	if (pBits == NULL)
		return;

	//���ڴ�д��ͼ������,����֤�Ƿ�����
	if (fread(pBits, ImageSize, 1, pFile) != 1) 
	{
		delete(pBits);
		return;
	}

	//�������
	fclose(pFile);
}