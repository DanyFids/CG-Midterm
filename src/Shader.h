#pragma once
#include<string>
#include<GLM/glm.hpp>
#include<glad/glad.h>

class Shader {

	int LoadShaderCode(const char* filename, GLchar** SourceCode);

public:
	//typedef std::shared_ptr<Shader> Sptr;

	unsigned int ID;

	Shader(const char* vertShaderPath, const char* fragShaderPath, const char* geoShaderPath = "");
	~Shader();

	void Use();

	void SetB(const std::string name, bool value);
	void SetI(const std::string name, int value);
	void SetF(const std::string name, float value);
	void SetMat3(const std::string name, glm::mat3 value);
	void SetMat4(const std::string name, glm::mat4 value);
	void SetVec3(const std::string name, glm::vec3 value);
};