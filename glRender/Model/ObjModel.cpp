#include "ObjModel.h"

void model_sum::parse(const char* filename) 
{
	string line;                  //当前行的文本
	string list;
	string head;                  //当前行首个单词
	string previous_head = "v";   //上一行首个单词,初始化为v
	ifstream fin(filename);
	//当前处理的物体
	obj3dmodel obj;
	if (!fin)
		return;
	while (getline(fin, line))
	{
		//得到当前行首个单词
		istringstream sin(line);
		sin >> head;
		//保存上一个物体的信息，并清空obj数据，以便处理当前物体
		if (head[0] != 'f' && previous_head[0] == 'f') {
			objects_num.push_back(obj);
			obj.vetexes.clear();
			obj.venorms.clear();
			obj.vetextures.clear();
			obj.faces_vetex.clear();
			obj.faces_norm.clear();
			obj.faces_textrue.clear();
		}
		if (head[0] == 'v') {
			if (head[1] == 'n') {  //保存顶点法线
				Vec4 vn;
				sin >> vn.x >> vn.y >> vn.z;
				obj.venorms.push_back(vn);
			}
			else if (head[1] == 't') {  //保存顶点纹理坐标
				TexturePoint2 vt;
				sin >> vt.x >> vt.y;
				obj.vetextures.push_back(vt);
			}
			else {                  //保存顶点坐标
				Point4 v;
				sin >> v.x >> v.y >> v.z;
				v.w = 1.0;
				if (v.x > obj.Vodimax.x)
					obj.Vodimax.x = v.x;             //记录包围模型的box，以便设置相机位置
				if (v.y > obj.Vodimax.y)
					obj.Vodimax.y = v.y;
				if (v.z > obj.Vodimax.z)
					obj.Vodimax.z = v.z;
				if (v.x < obj.Vodimin.x)
					obj.Vodimin.x = v.x;
				if (v.y < obj.Vodimin.y)
					obj.Vodimin.y = v.y;
				if (v.z < obj.Vodimin.z)
					obj.Vodimin.z = v.z;
				obj.vetexes.push_back(v);
			}
		}
		if (head[0] == 'f')
		{
			obj3dmodel::face faceVetex, faceTexture, faceNorm;
			unsigned int index;
			auto pos = line.find("//");
			if (pos != string::npos)   //检测是否省略了纹理坐标索引
			{
				obj.have_texture_ordi = false;
				for (int i = 0; i < 3; ++i) { //依不同obj文件中，f由3或4个顶点组成，i上限取<3或<4
					sin >> list;
					index = 0;   //处理顶点索引
					decltype(list.size()) k = 0;
					for (; list[k] != '/'; ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceVetex.d[i] = index;

					index = 0;   //跳过纹理部分，处理法线索引
					for (k = k + 2; k != list.size(); ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceNorm.d[i] = index;
				}
				obj.faces_vetex.push_back(faceVetex);
				obj.faces_norm.push_back(faceNorm);
			}
			else {                  //此处开始是未省略纹理坐标索引
				for (int i = 0; i < 3; ++i) {  //依不同obj文件中,f由3或4个顶点组成，i上限取<3或<4
					sin >> list;

					index = 0;   //处理顶点索引
					decltype(list.size()) k = 0;
					for (; list[k] != '/'; ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceVetex.d[i] = index;

					index = 0;   //处理纹理索引
					for (k = k + 1; list[k] != '/'; ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceTexture.d[i] = index;

					index = 0;   //处理法线索引
					for (k = k + 1; k != list.size(); ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceNorm.d[i] = index;
				}
				obj.faces_vetex.push_back(faceVetex);
				obj.faces_textrue.push_back(faceTexture);
				obj.faces_norm.push_back(faceNorm);
			}
		}
		previous_head = head;
	}
}
