#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <string>

using namespace std;

class Environment
{
public:
	static Environment* getInstance();

	bool isSupportAnisotropic();

private:
	Environment();
	void init();
	bool queryExtensionSupport(const char* name);

	static Environment* m_instance;
	vector<string> m_ExtensionsStrList;
	bool m_SupportAnisotropic;
};

#endif // !ENVIRONMENT_H
