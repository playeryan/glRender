#include "ObjModel.h"

void model_sum::parse(const char* filename) 
{
	string line;                  //��ǰ�е��ı�
	string list;
	string head;                  //��ǰ���׸�����
	string previous_head = "v";   //��һ���׸�����,��ʼ��Ϊv
	ifstream fin(filename);
	//��ǰ���������
	obj3dmodel obj;
	if (!fin)
		return;
	while (getline(fin, line))
	{
		//�õ���ǰ���׸�����
		istringstream sin(line);
		sin >> head;
		//������һ���������Ϣ�������obj���ݣ��Ա㴦��ǰ����
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
			if (head[1] == 'n') {  //���涥�㷨��
				Vec4 vn;
				sin >> vn.x >> vn.y >> vn.z;
				obj.venorms.push_back(vn);
			}
			else if (head[1] == 't') {  //���涥����������
				TexturePoint2 vt;
				sin >> vt.x >> vt.y;
				obj.vetextures.push_back(vt);
			}
			else {                  //���涥������
				Point4 v;
				sin >> v.x >> v.y >> v.z;
				v.w = 1.0;
				if (v.x > obj.Vodimax.x)
					obj.Vodimax.x = v.x;             //��¼��Χģ�͵�box���Ա��������λ��
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
			if (pos != string::npos)   //����Ƿ�ʡ����������������
			{
				obj.have_texture_ordi = false;
				for (int i = 0; i < 3; ++i) { //����ͬobj�ļ��У�f��3��4��������ɣ�i����ȡ<3��<4
					sin >> list;
					index = 0;   //����������
					decltype(list.size()) k = 0;
					for (; list[k] != '/'; ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceVetex.d[i] = index;

					index = 0;   //���������֣�����������
					for (k = k + 2; k != list.size(); ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceNorm.d[i] = index;
				}
				obj.faces_vetex.push_back(faceVetex);
				obj.faces_norm.push_back(faceNorm);
			}
			else {                  //�˴���ʼ��δʡ��������������
				for (int i = 0; i < 3; ++i) {  //����ͬobj�ļ���,f��3��4��������ɣ�i����ȡ<3��<4
					sin >> list;

					index = 0;   //����������
					decltype(list.size()) k = 0;
					for (; list[k] != '/'; ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceVetex.d[i] = index;

					index = 0;   //������������
					for (k = k + 1; list[k] != '/'; ++k) {
						index = index * 10 + (list[k] - 48);
					}
					faceTexture.d[i] = index;

					index = 0;   //����������
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
