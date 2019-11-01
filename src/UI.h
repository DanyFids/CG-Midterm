#pragma once
#include<string>
#include<vector>
#include<GLM/glm.hpp>
class Mesh;
class Material;
class Shader;
class Camera;

class UI {
	static Mesh* QUAD;
	static glm::mat4 projection;
	glm::mat4 model;
	Material* material;

public:
	static Shader* SHADER;

	static void INIT();

	UI(int width, int height, glm::vec2 pos, Material* ma);
	virtual void Draw(glm::vec2 scrn);
};