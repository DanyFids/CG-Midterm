#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>

#include"Game.h"

//void Draw(Shader* shaderObj);
//void Draw(unsigned int VAO, unsigned int shaderObj);
//void Update();
//int LoadShaders(unsigned int& VBO, unsigned int& shaderObj);
//int LoadShaderCode(const char* filename, GLchar** SourceCode);

int main() {
	Game Instance = Game();

	Instance.Run();
}

/*void Draw(Shader* shaderObj) {
	
}

/*void Draw(unsigned int VAO, unsigned int shaderObj)
{
	glBindVertexArray(VAO);
	glUseProgram(shaderObj);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}*/

/*
int LoadShaders(unsigned int & VAO, unsigned int & shaderObj) {
	// Shader Source Files
	const char* vertexShaderSourceFile = "Shaders/Basic_Shader.vert"; // Vertex Shader
	const char* fragShaderSourceFile = "Shaders/Basic_Shader.frag";   // Fragment Shader

	// Shader Source Code
	GLchar* vertexShaderSource ="#version 400\n"
        "in vec3 vpos;"
        "void main() {"
        "    gl_Position = vec4(vpos, 1.0);"
		"}";;
	GLchar* fragShaderSource = "#version 400\n"
        "out vec4 frag_color;"
        "void main() {"
        "    frag_color = vec4(0.3, 0.3, 1.0, 1.0);"
		"}";

	//Compilation Info
	int success;
	char infoLog[512];

	//if (LoadShaderCode(vertexShaderSourceFile, &vertexShaderSource)) return -5;
	//if (LoadShaderCode(fragShaderSourceFile, &fragShaderSource)) return -5;



	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Creating a Vertex Buffer, contains vertex information
	unsigned int VBO;
	glGenBuffers(1, &VBO); // Gen Buffer
	// Bind an Array buffer to our buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Load Buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriangles), twoTriangles, GL_STATIC_DRAW); // Loads buffer with triangle vertices

	// instruct openGL on how it should interpret vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		return -1;
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
		return -2;
	}

	// Shader Object, this links the shaders.
	shaderObj = glCreateProgram();

	glAttachShader(shaderObj, vertexShader);
	glAttachShader(shaderObj, fragShader);

	glLinkProgram(shaderObj);
	// Check program linked properly
	glGetProgramiv(shaderObj, GL_LINK_STATUS, &success);
	if (!success) { // Check if it was a success
		glGetProgramInfoLog(shaderObj, 512, NULL, infoLog); // Get Info Log
		std::cout << "ERROR::SHADER::PROG::LINK_FAILED\n" << infoLog << std::endl; // Output log
		return -3;
	}

	glUseProgram(shaderObj);

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return 0;
}
/*
int LoadShaderCode(const char* filename, GLchar** SourceCode)
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

	*SourceCode = (GLchar*) new char [len + (long)1];
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
}*/