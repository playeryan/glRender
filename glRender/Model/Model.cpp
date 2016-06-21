#include "Model.h"

GLuint Model::m_quadVAO = 0;
GLuint Model::m_quadVBO = 0;

Model::Mesh::Mesh()
	:	m_VertexArray(INVALID_GLRENDER_VALUE)
	,	m_VertexBuffer(INVALID_GLRENDER_VALUE)
	,	m_IndexBuffer(INVALID_GLRENDER_VALUE)
	,	m_NumIndices(0)
	,	m_MaterialIndex(INVALID_MATERIAL)
{}

Model::Mesh::~Mesh()
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

Model::Model()
	:	m_SceneCenterPos(Point3(0.0f, 0.0f, 0.0f))
{}

Model::~Model()
{
	Clear();
}

bool Model::LoadModel(const std::string & fileName)
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

void Model::Render(GeometryBufferShader* shaderobject)
{
	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		const unsigned int materialIndex = m_Meshes[i].m_MaterialIndex;
		if (m_diffuseTextures[materialIndex])
		{
			m_diffuseTextures[materialIndex]->Bind(TextureUnit::Diffuse);
		}
		if (m_specularTextures[materialIndex])
		{
			m_specularTextures[materialIndex]->Bind(TextureUnit::Specular);
		}
		if (m_heightTextures[materialIndex])
		{
			m_heightTextures[materialIndex]->Bind(TextureUnit::Height);
		}
		if (m_opacityTextures[materialIndex])
		{
			m_opacityTextures[materialIndex]->Bind(TextureUnit::Opacity);
			shaderobject->SetHasOpacity(GL_TRUE);
		}
		else
		{
			shaderobject->SetHasOpacity(GL_FALSE);
		}

		glBindVertexArray(m_Meshes[i].m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_Meshes[i].m_NumIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// unbind texture unit
		if (materialIndex < m_diffuseTextures.size() && m_diffuseTextures[materialIndex])
		{
			m_diffuseTextures[materialIndex]->UnBind(TextureUnit::Diffuse);
		}
		if (materialIndex < m_specularTextures.size() && m_specularTextures[materialIndex])
		{
			m_specularTextures[materialIndex]->UnBind(TextureUnit::Specular);
		}
		if (materialIndex < m_heightTextures.size() && m_heightTextures[materialIndex])
		{
			m_heightTextures[materialIndex]->UnBind(TextureUnit::Height);
		}
		//if (materialIndex < m_opacityTextures.size() && m_opacityTextures[materialIndex])
		//{
		//	m_heightTextures[materialIndex]->UnBind(TextureUnit::Opacity);
		//}
	}
}

bool Model::InitFromScene(const aiScene * pScene, const std::string & fileName)
{
	m_Meshes.resize(pScene->mNumMeshes);
	m_diffuseTextures.resize(pScene->mNumMaterials);
	m_specularTextures.resize(pScene->mNumMaterials);
	m_heightTextures.resize(pScene->mNumMaterials);		
	m_opacityTextures.resize(pScene->mNumMaterials);	//仔细考虑，各类型texture数量不一定相等，此处resize参数也需有依据/
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		m_diffuseTextures[i] = NULL;
		m_heightTextures[i] = NULL;
		m_heightTextures[i] = NULL;
		m_opacityTextures[i] = NULL;
	}

	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, fileName);
}

void Model::InitMesh(unsigned int index, const aiMesh * paiMesh)
{
	m_Meshes[index].m_MaterialIndex = paiMesh->mMaterialIndex;

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

	m_Meshes[index].Init(Vertices, Indices);
}

// 仿照loadMaterialTextures(...)来写/
bool Model::InitMaterials(const aiScene * pScene, const std::string & fileName)
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

		//if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0)
		//{
		//	printf("current material diffuse result: %d\n", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_AMBIENT) != 0)
		//{
		//	printf("current material ambient result: %d\n", pMaterial->GetTextureCount(aiTextureType_AMBIENT));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) != 0)
		//{
		//	printf("current material specular result: %d\n", pMaterial->GetTextureCount(aiTextureType_SPECULAR));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_HEIGHT) != 0)
		//{
		//	printf("current material height result: %d\n", pMaterial->GetTextureCount(aiTextureType_HEIGHT));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_NONE) != 0)
		//{
		//	printf("current material none result: %d\n", pMaterial->GetTextureCount(aiTextureType_NONE));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_EMISSIVE) != 0)
		//{
		//	printf("current material emissive result: %d\n", pMaterial->GetTextureCount(aiTextureType_EMISSIVE));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_NORMALS) != 0)
		//{
		//	printf("current material normals result: %d\n", pMaterial->GetTextureCount(aiTextureType_NORMALS));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_SHININESS) != 0)
		//{
		//	printf("current material shininess result: %d\n", pMaterial->GetTextureCount(aiTextureType_SHININESS));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_OPACITY) != 0)
		//{
		//	printf("current material opacity result: %d\n", pMaterial->GetTextureCount(aiTextureType_OPACITY));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT) != 0)
		//{
		//	printf("current material displacement result: %d\n", pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_LIGHTMAP) != 0)
		//{
		//	printf("current material lightmap result: %d\n", pMaterial->GetTextureCount(aiTextureType_LIGHTMAP));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_REFLECTION) != 0)
		//{
		//	printf("current material reflection result: %d\n", pMaterial->GetTextureCount(aiTextureType_REFLECTION));
		//}
		//if (pMaterial->GetTextureCount(aiTextureType_UNKNOWN) != 0)
		//{
		//	printf("current material unknown result: %d\n", pMaterial->GetTextureCount(aiTextureType_UNKNOWN));
		//}

		InitVariousMaterials(pMaterial, i, aiTextureType_DIFFUSE, Dir);
		InitVariousMaterials(pMaterial, i, aiTextureType_SPECULAR, Dir);
		InitVariousMaterials(pMaterial, i, aiTextureType_HEIGHT, Dir);
		InitVariousMaterials(pMaterial, i, aiTextureType_OPACITY, Dir);
	}

	return flag;
}

bool Model::InitVariousMaterials(const aiMaterial * pMaterial, unsigned int index, aiTextureType type, const std::string & dir)
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
		case aiTextureType_SPECULAR:
			reference = m_specularTextures.begin();
			strType = SpecularTexture;
			break;
		case aiTextureType_OPACITY:
			reference = m_opacityTextures.begin();
			strType = OpacityTexture;
			break;
		default:
			strType = NoneTexture;
			break;
		}
		// 这里认为1个material仅对应1个texture，故index参数取0。是否不严谨？待斟酌/
		if (pMaterial->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string fullPath = dir + "/" + path.data;
			//if (type == aiTextureType_OPACITY)
			//{
				printf("materail(%d), path: %s\n", index, fullPath.c_str());
			//}
			
			Texture* tex = new Texture(fullPath, GL_TEXTURE_2D);
			
			if (!tex->Load())
			{
				printf("Error when loading texture: '%s'\n", fullPath.c_str());
				SAFE_DELETE_POINTER(tex);
			}
			tex->SetTextureType(strType);
			*(reference + index) = tex;		// put each texture's pointer into vector

			result = true;
		}
	}
	return result;
}

void Model::CalcSceneCenterPos(const aiScene * pScene)
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
	m_SceneCenterPos.x = vertexMaxX + vertexMinX;
	m_SceneCenterPos.y = (vertexMaxY + vertexMinY) / 2.0f; // 无需太高/
	m_SceneCenterPos.z = vertexMaxZ + vertexMinZ;

	m_SceneMaxPos.x = vertexMaxX;
	m_SceneMaxPos.y = vertexMaxY;
	m_SceneMaxPos.z = vertexMaxZ;
	//printf("Scene max vertex: (%f, %f, %f)\n", vertexMaxX, vertexMaxY, vertexMaxZ);
	//printf("Scene min vertex: (%f, %f, %f)\n", vertexMinX, vertexMinY, vertexMinZ);
	//printf("Scene center pos: (%f, %f, %f)\n", m_SceneCenterPos.x, m_SceneCenterPos.y, m_SceneCenterPos.z);
}

float Model::getSuitableDistanceFactor()
{
	float maxX = m_SceneMaxPos.x > 0.0f ? m_SceneMaxPos.x : -m_SceneMaxPos.x;
	float maxY = m_SceneMaxPos.y > 0.0f ? m_SceneMaxPos.y : -m_SceneMaxPos.y;
	float maxZ = m_SceneMaxPos.z > 0.0f ? m_SceneMaxPos.z : -m_SceneMaxPos.z;

	float result = (maxX + maxY + maxZ) / 3.0f * 0.2f;
	return result > 0.0f ? result : -result;
}

void Model::RenderQuad()
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

Point3 Model::GetSceneCenterPos()
{
	return m_SceneCenterPos;
}

void Model::Clear()
{
	for (size_t i = 0; i < m_diffuseTextures.size(); i++)
	{
		SAFE_DELETE_POINTER(m_diffuseTextures[i]);
	}
}

void Model::Mesh::Init(const std::vector<VertexAttribute>& Vertices, const std::vector<unsigned int>& Indices)
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

Model::MaterialProperty::MaterialProperty()
	:	m_ambientColor(Vec3(0.0, 0.0, 0.0))
	,	m_diffuseColor(Vec3(0.0, 0.0, 0.0))
	,	m_specularColor(Vec3(0.0, 0.0, 0.0))
	,	m_emissveColor(Vec3(0.0, 0.0, 0.0))
{
}

Model::MaterialProperty::~MaterialProperty()
{
}

void Model::MaterialProperty::Init(const aiColor3D ambient, const aiColor3D diffuse, const aiColor3D specular, const aiColor3D emissive)
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
