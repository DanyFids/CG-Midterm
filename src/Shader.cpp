#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <GLM/gtc/type_ptr.hpp>
#include<iostream>
#include<fstream>
#include "Shader.h"

int Shader::LoadShaderCode(const char* filename, GLchar** SourceCode)
{
	// Open File
	std::ifstream file;
	file.open(filename, std::ios::in);
	if (!file) return -1;

	if (!file.good()) return -2;

	// Find File length
	unsigned long pos = file.tellg();
	file.seekg(0, std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	if (len == 0) return -3;

	*SourceCode = (GLchar*) new char[len + (long)1];
	if (*SourceCode == 0) return -4;

	(*SourceCode)[len] = 0;

	unsigned int i = 0;
	while (file.good())
	{
		(*SourceCode)[i] = file.get();
		if (!file.eof())
			i++;
	}

	(*SourceCode)[i] = 0;

	file.close();

	return 0;
}

Shader::Shader(const char* vertShaderPath, const char* fragShaderPath, const char* geoShaderPath)
{
	ID = 0;

	// Shader Source Code
	GLchar* vertexShaderSource;
	GLchar* fragShaderSource;
	GLchar* geoShaderSource;

	//Compilation Info
	int success;
	char infoLog[512];

	if (int err = LoadShaderCode(vertShaderPath, &vertexShaderSource)) {
		std::cout << "ERROR::SHADER::VERTEX::ERROR_READING_FILE\nERROR_CODE: " << err << std::endl;
		return;
	}
	if (int err = LoadShaderCode(fragShaderPath, &fragShaderSource)) {
		std::cout << "ERROR::SHADER::FRAG::ERROR_READING_FILE\nERROR_CODE: " << err << std::endl;
		return;
	}

	// Compiling Vertex Shader, for 3D geometry.
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check that Shader compiled properly
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) { // Check if it was a success
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get Info Log
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; // Output log
		return;
	}

	// Compiling Fragment Shader, for pixel manipulation
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);
	// Check that Shader compiled properly
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) { // Check if it was a success
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog); // Get Info Log
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl; // Output log
		return;
	}

	// Shader Object, this links the shaders.
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);

	unsigned int geoShader = 0;
	if (geoShaderPath != "") {
		geoShader = glCreateShader(GL_GEOMETRY_SHADER);

		if (int err = LoadShaderCode(geoShaderPath, &geoShaderSource)) {
			std::cout << "ERROR::SHADER::GEO::ERROR_READING_FILE\nERROR_CODE: " << err << std::endl;
			return;
		}

		glShaderSource(geoShader, 1, &geoShaderSource, NULL);
		glCompileShader(geoShader);
		glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEO::COMPILATION_FAILED\n" << infoLog << std::endl; // Output log
			return;
		}

		glAttachShader(ID, geoShader);
	}

	glAttachShader(ID, fragShader);

	glLinkProgram(ID);
	// Check program linked properly
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) { // Check if it was a success
		glGetProgramInfoLog(ID, 512, NULL, infoLog); // Get Info Log
		std::cout << "ERROR::SHADER::PROG::LINK_FAILED\n" << infoLog << std::endl; // Output log
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	if (geoShaderPath != "") {
		glDeleteShader(geoShader);
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetB(const std::string name, bool value)
{
	Use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetI(const std::string name, int value)
{
	Use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetF(const std::string name, float value)
{
	Use();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMat3(const std::string name, glm::mat3 value)
{
	Use();
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string name, glm::mat4 value)
{
	Use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string name, glm::vec3 value)
{
	Use();
	int loc = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(loc, 1, &value[0]);
}

