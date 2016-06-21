#include <GL/glew.h>
#include <cstdio>
#include <fstream>
#include "Environment.h"

Environment* Environment::m_instance = NULL;

Environment * Environment::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Environment();
		m_instance->init();
	}

	return m_instance;
}

bool Environment::isSupportAnisotropic()
{
	return m_SupportAnisotropic;
}

void Environment::init()
{
	GLint extensionNum = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extensionNum);
	printf("extension nums: %d\n", extensionNum);
	for (int i = 0; i < extensionNum; i++)
	{
		const GLubyte* extensionStr = glGetStringi(GL_EXTENSIONS, i);
		size_t longth = strlen((const char*)extensionStr);
		char* extensionName = new char[longth + 1];		// sprintf()会自动向char数组末尾写入一个结束符‘\0’，所以长度需要+1/
		sprintf(extensionName, "%s", extensionStr);
		m_ExtensionsStrList.push_back(string(extensionName));
		delete[] extensionName;
		extensionName = NULL;
	}
	
	ofstream file("extension list.txt", ios::binary);
	for (size_t i = 0; i < m_ExtensionsStrList.size(); i++)
	{
		file << m_ExtensionsStrList[i];
		file << '\n';
	}

	// 记录所需的extension是否支持/
	m_SupportAnisotropic = queryExtensionSupport("GL_EXT_texture_filter_anisotropic");
}

bool Environment::queryExtensionSupport(const char * name)
{
	bool result = false;
	string extName(name);
	for (size_t i = 0; i < m_ExtensionsStrList.size(); i++)
	{
		if (extName == m_ExtensionsStrList[i])
		{
			result = true;
		}
	}

	if (result)
	{
		printf("%s, support\n", name);
	}
	else
	{
		printf("%s, not support\n", name);
	}

	return result;
}

Environment::Environment()
	:	m_SupportAnisotropic(false)
{

}