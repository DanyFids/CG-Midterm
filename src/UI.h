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
	Material* material;
	glm::vec2 dim;
	glm::vec2 position;

public:
	static Shader* SHADER;

	UI(int width, int height, glm::vec2 pos, Material* ma);
	virtual void Draw(glm::vec2 scrn);
};