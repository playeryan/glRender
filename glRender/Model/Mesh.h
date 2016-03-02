#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>
#include <gl\glew.h>
#include <Math\Vec4.h>
#include <Math\Vec3.h>
#include <Math\MathMacros.h>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include "../Common/Texture.h"
#include "../Macros/glMacros.h"

enum Attrib_IDs
{
	vPosition = 0,
	vTexCooord,
	vNormal,
};

struct VertexAttribute
{
	VertexAttribute() = default;
	VertexAttribute(const Point3 pos, const TexturePoint2 tex, const Vec3 normal)
		:	m_pos(pos)
		,	m_tex(tex)
		,	m_normal(normal)
	{}

	Point3			m_pos;
	TexturePoint2	m_tex;
	Vec3			m_normal;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& fileName);

	void Render();

	Point3 GetSceneCenterPos();
	float getSuitableDistanceFactor();
	

private:
	bool InitFromScene(const aiScene* pScene, const std::string& fileName);
	void InitMesh(unsigned int index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& fileName);
	void CalcSceneCenterPos(const aiScene* pScene);	// 结构优化空间：移至具体init mesh时，循环计算/
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

	struct MeshEntry
	{
		MeshEntry();
		~MeshEntry();

		void Init(const std::vector<VertexAttribute>& Vertices,
				  const std::vector<unsigned int>& Indices);

		GLuint m_VertexBuffer;
		GLuint m_IndexBuffer;
		unsigned int m_NumIndices;
		unsigned int m_MaterialIndex;
	};

	struct MaterialProperty
	{
		MaterialProperty();
		~MaterialProperty();

		void Init(const aiColor3D ambient, const aiColor3D diffuse, const aiColor3D specular, const aiColor3D emissive);
		Vec3 m_ambientColor;
		Vec3 m_diffuseColor;
		Vec3 m_specularColor;
		Vec3 m_emissveColor;
	};

	std::vector<MeshEntry>			m_Entries;	// 优化空间：容器存放指针，而不是数据实例/
	std::vector<Texture*>			m_Textures;
	std::vector<MaterialProperty>	m_materialProperty;
	Point3 m_SceneCenterPos;
	Point3 m_SceneMaxPos;
};



#endif // !MESH_H

