#pragma once
#include<GLM/glm.hpp>
#include<vector>
class Material;
class Shader;
class Camera;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_uv;
	glm::vec3 tangent;
	glm::vec3 biTan;
};

class Mesh {
	unsigned int vao;
	unsigned int buffers[2];
	int num_indices;
	Material * tex;
	glm::mat4 model;


public:
	Mesh(float vert[], int num_vert, unsigned int indi[], int num_indi);
	Mesh(const char* file);
	~Mesh();

	void Draw(Shader*, std::vector<Camera*>);
	void SetTexture(Material* t);
	void Update(float dt);

	void LoadMesh(const char* f, std::vector<Vertex> &vertices, unsigned int& num_verts, std::vector<unsigned int> &indices, unsigned int& num_indi);

	void Move(glm::vec3 dir, float dt);
	void SetPosition(glm::vec3 pos);
};