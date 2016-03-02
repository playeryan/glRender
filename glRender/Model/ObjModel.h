#pragma once
#ifndef MYOBJ_H
#define MYOBJ_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Math/Vec4.h"
#include "Math/Vec3.h"

//��¼��������ĸ�����Ϣ
struct obj3dmodel 
{
	obj3dmodel() = default;

	Point4 Vodimax = { 0, 0, 0 };  
	Point4 Vodimin = { 0, 0, 0 };  //Ԥ��ģ�ʹ�С�������������λ��
	//
	bool have_texture_ordi = true;
	//������������Ϣ������������
	struct face {
		int d[3] = { 0,0,0 };
	};

	std::vector<Point4> vetexes;
	std::vector<Vec4> venorms;
	std::vector<TexturePoint2> vetextures;
	std::vector<face> faces_vetex;
	std::vector<face> faces_textrue;
	std::vector<face> faces_norm;
};

//
struct model_sum 
{
	vector<obj3dmodel> objects_num;
	void parse(const char *);
};

#endif
