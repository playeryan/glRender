#include "mesh.h"

GLuint Mesh::m_quadVAO = 0;
GLuint Mesh::m_quadVBO = 0;

Mesh::MeshEntry::MeshEntry()
	:	m_VertexArray(INVALID_GLRENDER_VALUE)
	,	m_VertexBuffer(INVALID_GLRENDER_VALUE)
	,	m_IndexBuffer(INVALID_GLRENDER_VALUE)
	,	m_NumIndices(0)
	,	m_MaterialIndex(INVALID_MATERIAL)
{}

Mesh::MeshEntry::~MeshEntry()
{
	if (m_VertexArray != INVALID_GLRENDER_VALUE)
	{
		glDeleteVertexArrays(1, &m_VertexArray);
	}
	if (m_VertexBuffer != INVALID_GLRENDER_VALUE)
	{
		glDeleteBuffers(1, &m_VertexBuffer);
	}
	if (m_IndexBuffer != INVALID_GLRENDER_VALUE)
	{
		glDeleteBuffers(1, &m_IndexBuffer);
	}
}

Mesh::Mesh()
	:	m_SceneCenterPos(Point3(0.0f, 0.0f, 0.0f))
{}

Mesh::~Mesh()
{
	Clear();
}

bool Mesh::LoadMesh(const std::string & fileName)
{
	Clear();
	
	bool flag = false;

	Assimp::Importer Importer;
	// 读取时可自由选择参数/
	// 此处依次为：
	// 将非三角形组成的模型转换为三角面组成的模型/
	// 为不含顶点法线的模型生成顶点法线/
	// 沿Y轴翻转纹理坐标/
	const aiScene* pScene = Importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene)
	{
		printf("read model file success, next to init scene\n");
		flag = InitFromScene(pScene, fileName);
		CalcSceneCenterPos(pScene);
	}
	else
	{
		printf("Error when parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
	}

	return flag;
}

void Mesh::Render()
{
	for (size_t i = 0; i < m_Entries.size(); i++)
	{
		const unsigned int materialIndex = m_Entries[i].m_MaterialIndex;
		if (materialIndex < m_diffuseTextures.size() && m_diffuseTextures[materialIndex])
		{
			m_diffuseTextures[materialIndex]->Bind(GL_TEXTURE0);
			// do something about multi material/
		}

		glBindVertexArray(m_Entries[i].m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_Entries[i].m_NumIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// unbind texture unit
		if (materialIndex < m_diffuseTextures.size() && m_diffuseTextures[materialIndex])
		{
			m_diffuseTextures[materialIndex]->UnBind(GL_TEXTURE0);
		}
	}
}

bool Mesh::InitFromScene(const aiScene * pScene, const std::string & fileName)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_diffuseTextures.resize(pScene->mNumMaterials);
	m_heightTextures.resize(pScene->mNumMaterials);
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		m_diffuseTextures[i] = NULL;
		m_heightTextures[i] = NULL;
	}

	for (size_t i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, fileName);
}

void Mesh::InitMesh(unsigned int index, const aiMesh * paiMesh)
{
	m_Entries[index].m_MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<VertexAttribute> Vertices;
	std::vector<unsigned int> Indices;
	

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		// 某顶点可能有多个纹理坐标，这里待扩展/
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		//const aiVector3D* pTangent = &(paiMesh->mTangents[i]);
		//const aiVector3D* pBitTangent = &(paiMesh->mBitangents[i]);

		VertexAttribute va(	Point3(pPos->x, pPos->y, pPos->z), 
							TexturePoint2(pTexCoord->x, pTexCoord->y), 
							Vec3(pNormal->x, pNormal->y, pNormal->z)/*,
							Vec3(pTangent->x, pTangent->y, pTangent->z),
							Vec3(pBitTangent->x, pBitTangent->y, pBitTangent->z) */);
		Vertices.push_back(va);
	}

	for (size_t i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace* paiFace = &(paiMesh->mFaces[i]);
		assert(paiFace->mNumIndices == 3);
		Indices.push_back(paiFace->mIndices[0]);
		Indices.push_back(paiFace->mIndices[1]);
		Indices.push_back(paiFace->mIndices[2]);
	}

	m_Entries[index].Init(Vertices, Indices);
}

bool Mesh::InitMaterials(const aiScene * pScene, const std::string & fileName)
{
	std::string::size_type SlashIndex = fileName.find_last_of("/");
	std::string Dir;
	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = fileName.substr(0, SlashIndex);
	}

	bool flag = true;
	
	printf("material nums: %d\n", pScene->mNumMaterials);
	m_materialProperty.resize(pScene->mNumMaterials);
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		aiColor3D ambientColor(0.0, 0.0, 0.0);
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		aiColor3D diffuseColor(0.0, 0.0, 0.0);
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		aiColor3D specularColor(0.0, 0.0, 0.0);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		aiColor3D emissiveColor(0.0, 0.0, 0.0);
		pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);

		m_materialProperty[i].Init(ambientColor, diffuseColor, specularColor, emissiveColor);

		printf("current material diffuse result: %d\n", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
		printf("current material ambient result: %d\n", pMaterial->GetTextureCount(aiTextureType_AMBIENT));
		printf("current material specular result: %d\n", pMaterial->GetTextureCount(aiTextureType_SPECULAR));
		printf("current material height result: %d\n", pMaterial->GetTextureCount(aiTextureType_HEIGHT));

		InitVariousMaterials(pMaterial, i, aiTextureType_DIFFUSE, Dir);
		InitVariousMaterials(pMaterial, i, aiTextureType_HEIGHT, Dir);
	}

	return flag;
}

bool Mesh::InitVariousMaterials(const aiMaterial * pMaterial, unsigned int index, aiTextureType type, const std::string & dir)
{
	bool result = false;
	if (pMaterial->GetTextureCount(type) > 0)
	{
		aiString path;
		std::vector<Texture*>::iterator reference;
		std::string strType;
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			reference = m_diffuseTextures.begin();
			strType = DiffuseTexture;
			break;
		case aiTextureType_HEIGHT:
			reference = m_heightTextures.begin();
			strType = HeightTexture;
			break;
		default:
			strType = NoneTexture;
			break;
		}
		// 这里认为1个material仅对应1个texture，故index参数取0。是不严谨的，待修改/
		if (pMaterial->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string fullPath = dir + "/" + path.data;
			printf("materail(%d), path: %s\n", index, fullPath.c_str());
			
			Texture* tex = new Texture(fullPath, GL_TEXTURE_2D);
			
			if (!tex->Load())
			{
				printf("Error when loading texture: '%s'\n", fullPath.c_str());
				SAFE_DELETE_POINTER(tex);
			}
			tex->SetTextureType(strType);
			*(reference + index) = tex;

			result = true;
		}
	}
	return result;
}

void Mesh::CalcSceneCenterPos(const aiScene * pScene)
{
	float vertexMaxX = 0.0f, vertexMaxY = 0.0f, vertexMaxZ = 0.0f;
	float vertexMinX = 100.0f, vertexMinY = 100.0f, vertexMinZ = 100.0f;
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh* paiMesh = pScene->mMeshes[i];
		for (size_t i = 0; i < paiMesh->mNumVertices; i++)
		{
			aiVector3D* vertex = &(paiMesh->mVertices[i]);
			if (vertex->x > vertexMaxX)
			{
				vertexMaxX = vertex->x;
			}
			if (vertex->y > vertexMaxY)
			{
				vertexMaxY = vertex->y;
			}
			if (vertex->z > vertexMaxZ)
			{
				vertexMaxZ = vertex->z;
			}

			if (vertexMinX > vertex->x)
			{
				vertexMinX = vertex->x;
			}
			if (vertexMinY > vertex->y)
			{
				vertexMinY = vertex->y;
			}
			if (vertexMinZ > vertex->z)
			{
				vertexMinZ = vertex->z;
			}
		}
	}
	printf("Scene max vertex: (%f, %f, %f)\n", vertexMaxX, vertexMaxY, vertexMaxZ);
	printf("Scene min vertex: (%f, %f, %f)\n", vertexMinX, vertexMinY, vertexMinZ);
	m_SceneCenterPos.x = vertexMaxX + vertexMinX;
	m_SceneCenterPos.y = (vertexMaxY + vertexMinY) / 2.0f; // 无需太高/
	m_SceneCenterPos.z = vertexMaxZ + vertexMinZ;

	m_SceneMaxPos.x = vertexMaxX;
	m_SceneMaxPos.y = vertexMaxY;
	m_SceneMaxPos.z = vertexMaxZ;
	printf("Scene center pos: (%f, %f, %f)\n", m_SceneCenterPos.x, m_SceneCenterPos.y, m_SceneCenterPos.z);
}

float Mesh::getSuitableDistanceFactor()
{
	float maxX = m_SceneMaxPos.x > 0.0f ? m_SceneMaxPos.x : -m_SceneMaxPos.x;
	float maxY = m_SceneMaxPos.y > 0.0f ? m_SceneMaxPos.y : -m_SceneMaxPos.y;
	float maxZ = m_SceneMaxPos.z > 0.0f ? m_SceneMaxPos.z : -m_SceneMaxPos.z;

	float result = (maxX + maxY + maxZ) / 3.0f * 0.2f;
	return result > 0.0f ? result : -result;
}

void Mesh::RenderQuad()
{
	if (m_quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &m_quadVBO);
		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Point3 Mesh::GetSceneCenterPos()
{
	return m_SceneCenterPos;
}

void Mesh::Clear()
{
	for (size_t i = 0; i < m_diffuseTextures.size(); i++)
	{
		SAFE_DELETE_POINTER(m_diffuseTextures[i]);
	}
}

void Mesh::MeshEntry::Init(const std::vector<VertexAttribute>& Vertices, const std::vector<unsigned int>& Indices)
{
	if (Vertices.size() != 0 && Indices.size() != 0)
	{
		m_NumIndices = Indices.size();
		glGenVertexArrays(1, &m_VertexArray);
		glGenBuffers(1, &m_VertexBuffer);
		glGenBuffers(1, &m_IndexBuffer);

		glBindVertexArray(m_VertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_NumIndices, &Indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), BUFFER_OFFSET(0));

		glEnableVertexAttribArray(vTexCooord);
		//glVertexAttribPointer(vTexCooord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (const GLvoid*)(sizeof(Point3)));
		glVertexAttribPointer(vTexCooord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (const GLvoid*)offsetof(VertexAttribute, m_tex));

		glEnableVertexAttribArray(vNormal);
		//glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (const GLvoid*)(sizeof(Point3) + sizeof(TexturePoint2)));
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (const GLvoid*)offsetof(VertexAttribute, m_normal));

		// finish, unbind current vertex array
		glBindVertexArray(0);
	}
}

Mesh::MaterialProperty::MaterialProperty()
	:	m_ambientColor(Vec3(0.0, 0.0, 0.0))
	,	m_diffuseColor(Vec3(0.0, 0.0, 0.0))
	,	m_specularColor(Vec3(0.0, 0.0, 0.0))
	,	m_emissveColor(Vec3(0.0, 0.0, 0.0))
{
}

Mesh::MaterialProperty::~MaterialProperty()
{
}

void Mesh::MaterialProperty::Init(const aiColor3D ambient, const aiColor3D diffuse, const aiColor3D specular, const aiColor3D emissive)
{
	m_ambientColor = Vec3(ambient.r, ambient.g, ambient.b);
	m_diffuseColor = Vec3(diffuse.r, diffuse.g, diffuse.b);
	m_specularColor = Vec3(specular.r, specular.g, specular.b);
	m_emissveColor = Vec3(emissive.r, emissive.g, emissive.b);

	//printf("ambient(%f, %f, %f)\ndiffuse(%f, %f, %f)\nspecular(%f, %f, %f)\nemissive(%f, %f, %f)\n",
	//	m_ambientColor.x, m_ambientColor.y, m_ambientColor.z,
	//	m_diffuseColor.x, m_diffuseColor.y, m_diffuseColor.z,
	//	m_specularColor.x, m_specularColor.y, m_specularColor.z,
	//	m_emissveColor.x, m_emissveColor.y, m_emissveColor.z);
}
