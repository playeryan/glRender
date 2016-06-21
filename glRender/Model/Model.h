#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <map>
#include <gl\glew.h>
#include <Math\Vec4.h>
#include <Math\Vec3.h>
#include <Math\MathMacros.h>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <Common\ShaderObject.h>

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
		: m_pos(pos)
		, m_tex(tex)
		, m_normal(normal)
		//, m_tangent(tangent)
		//, m_bitTangent(bitTangent)
	{}

	Point3			m_pos;
	TexturePoint2	m_tex;
	Vec3			m_normal;
	//Vec3			m_tangent;
	//Vec3			m_bitTangent;
};

class Model
{
public:
	Model();
	~Model();

	bool LoadModel(const std::string& fileName);

	void Render(GeometryBufferShader* shaderobject);

	Point3 GetSceneCenterPos();
	float getSuitableDistanceFactor();
	
	// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
	// and post-processing effects.
	static void RenderQuad();
private:
	bool InitFromScene(const aiScene* pScene, const std::string& fileName);
	void InitMesh(unsigned int index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& fileName);
	bool InitVariousMaterials(const aiMaterial* pMaterial, unsigned int index, aiTextureType type, const std::string& dir);

	void CalcSceneCenterPos(const aiScene* pScene);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

	struct Mesh
	{
		Mesh();
		~Mesh();

		void Init(const std::vector<VertexAttribute>& Vertices,
				  const std::vector<unsigned int>& Indices);

		GLuint m_VertexArray;
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

	std::vector<Mesh>				m_Meshes;	
	std::vector<Texture*>			m_diffuseTextures;
	std::vector<Texture*>			m_specularTextures;
	std::vector<Texture*>			m_heightTextures;
	std::vector<Texture*>			m_opacityTextures;
	std::vector<MaterialProperty>	m_materialProperty;
	Point3 m_SceneCenterPos;
	Point3 m_SceneMaxPos;
private:
	static GLuint m_quadVAO;
	static GLuint m_quadVBO;
};



#endif // !MODEL_H

