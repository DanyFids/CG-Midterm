#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "Game.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"

Mesh::Mesh(float vert[], int num_vert, unsigned int indi[], int num_indi) {

	// setup Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// setup array Buffer
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vert, vert, GL_STATIC_DRAW);

	// setup element buffer
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indi, indi, GL_STATIC_DRAW);

	Vertex* temp_vert = nullptr;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->tex_uv));
	glEnableVertexAttribArray(2);

	num_indices = num_indi;

	// save buffers
	buffers[0] = vbo;
	buffers[1] = ebo;

	// Setup Transformation Matrices
	model = glm::mat4(1.0f); // world transformations
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//test transformations
	//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

Mesh::Mesh(const char* file)
{
	unsigned int num_vert, num_indi;
	

	std::vector<Vertex> vert_vec = std::vector<Vertex>();
	std::vector<unsigned int> indi_vec = std::vector<unsigned int>();

	LoadMesh(file, vert_vec, num_vert, indi_vec, num_indi);

	Vertex* vert;
	unsigned int* indi;
	vert = new Vertex[num_vert];
	indi = new unsigned int[num_indi];

	for (int c = 0; c < vert_vec.size(); c++) {
		vert[c] = vert_vec[c];
	}

	for (int c = 0; c < indi_vec.size(); c++) {
		indi[c] = indi_vec[c];
	}

	// setup Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// setup array Buffer
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vert, vert, GL_STATIC_DRAW);

	// setup element buffer
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indi, indi, GL_STATIC_DRAW);

	Vertex* temp_vert = nullptr;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->tex_uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(temp_vert->biTan));
	glEnableVertexAttribArray(4);

	num_indices = num_indi;

	// save buffers
	buffers[0] = vbo;
	buffers[1] = ebo;

	// Setup Transformation Matrices
	model = glm::mat4(1.0f); // world transformations
}

Mesh::~Mesh()
{
	
}

void Mesh::Draw(Shader* shader, std::vector<Camera*> cam)
{
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::SetTexture(Material* t) {
	tex = t;
}

void Mesh::Update(float dt) {
	model = glm::rotate(model, glm::radians(50.0f * dt), glm::vec3(0.5f, 1.0f, 1.0f));
}

void Mesh::LoadMesh(const char* f, std::vector<Vertex>& vertices, unsigned int& num_verts, std::vector<unsigned int>& indices, unsigned int& num_indi)
{
	std::ifstream file;
	file.open(f, std::ios::in);

	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	std::vector<glm::vec3> norms;
	std::vector<glm::vec2> texUVs;

	//std::vector<Vertex> vertices;

	std::string line;

	//std::vector<int> indices;

	while (!file.eof()) {
		if (file.peek() == '#') {
			file.ignore(512, (int)'\n');
			continue;
		}
		file >> line;

		if (line.compare("v") == 0) {
			glm::vec3 temp;

			file >> line;
			temp.x = std::stof(line);
			file >> line;
			temp.y = std::stof(line);
			file >> line;
			temp.z = std::stof(line);

			positions.push_back(glm::vec3(temp.x, temp.y, temp.z));
		}
		else if(line.compare("vn") == 0){
			glm::vec3 temp;

			file >> line;
			temp.x = std::stof(line);
			file >> line;
			temp.y = std::stof(line);
			file >> line;
			temp.z = std::stof(line);

			norms.push_back(glm::vec3(temp.x, temp.y, temp.z));
		}
		else if (line.compare("vt") == 0) {
			glm::vec2 temp;

			file >> line;
			temp.x = std::stof(line);
			file >> line;
			temp.y = std::stof(line);

			texUVs.push_back(glm::vec2(temp.x, temp.y));
		}
		else if (line.compare("f") == 0) {
			int count = 0;
			int base = vertices.size();

			while (file.peek() != '\n') {
				file >> line;

				int pos, nor, tex;

				std::istringstream data(line);
				std::string i;
				std::getline(data, i, '/');
				pos = std::stoi(i);
				std::getline(data, i, '/');
				tex = std::stoi(i);
				data >> i;
				nor = std::stoi(i);

				Vertex temp;
				temp.position = positions[pos-1];
				temp.normal = norms[nor-1];
				temp.tex_uv = texUVs[tex-1];

				vertices.push_back(Vertex(temp));
				count++;
			}

			if (count == 3) {
				indices.push_back(base);
				indices.push_back(base+1);
				indices.push_back(base+2);

				// calculating tangenty and bitangent lines
				glm::vec3 e1 = vertices[base + 1].position - vertices[base].position;
				glm::vec3 e2 = vertices[base + 2].position - vertices[base].position;
				glm::vec2 d1 = vertices[base + 1].tex_uv - vertices[base].tex_uv;
				glm::vec2 d2 = vertices[base + 2].tex_uv - vertices[base].tex_uv;

				glm::vec3 tangent;
				glm::vec3 biTan;

				float fac = 1.0f / (d1.x * d2.y - d2.x * d1.y);

				tangent.x = fac * (d2.y * e1.x - d1.y * e2.x);
				tangent.y = fac * (d2.y * e1.y - d1.y * e2.y);
				tangent.z = fac * (d2.y * e1.z - d1.y * e2.z);
				tangent = glm::normalize(tangent);

				biTan.x = fac * (-d2.x * e1.x + d1.x * e2.x);
				biTan.y = fac * (-d2.x * e1.y + d1.x * e2.y);
				biTan.z = fac * (-d2.x * e1.z + d1.x * e2.z);
				biTan = glm::normalize(biTan);

				vertices[base].tangent = tangent;
				vertices[base].biTan = biTan;
				vertices[base+1].tangent = tangent;
				vertices[base+1].biTan = biTan;
				vertices[base+2].tangent = tangent;
				vertices[base+2].biTan = biTan;
			}
			else if (count == 4) {
				indices.push_back(base);
				indices.push_back(base + 1);
				indices.push_back(base + 2);

				indices.push_back(base + 0);
				indices.push_back(base + 2);
				indices.push_back(base + 3);

				// calculating tangenty and bitangent lines
				// Tri 1
				glm::vec3 e1 = vertices[base].position - vertices[base+1].position;
				glm::vec3 e2 = vertices[base + 2].position - vertices[base+1].position;
				glm::vec2 d1 = vertices[base].tex_uv - vertices[base+1].tex_uv;
				glm::vec2 d2 = vertices[base + 2].tex_uv - vertices[base+1].tex_uv;

				glm::vec3 tangent1;
				glm::vec3 biTan1;

				float fac1 = 1.0f / (d1.x * d2.y - d2.x * d1.y);

				tangent1.x = fac1 * (d2.y * e1.x - d1.y * e2.x);
				tangent1.y = fac1 * (d2.y * e1.y - d1.y * e2.y);
				tangent1.z = fac1 * (d2.y * e1.z - d1.y * e2.z);
				tangent1 = glm::normalize(tangent1);

				biTan1.x = fac1 * (-d2.x * e1.x + d1.x * e2.x);
				biTan1.y = fac1 * (-d2.x * e1.y + d1.x * e2.y);
				biTan1.z = fac1 * (-d2.x * e1.z + d1.x * e2.z);
				biTan1 = glm::normalize(biTan1);

				// Tri 1
				glm::vec3 e3 = vertices[base].position - vertices[base+3].position;
				glm::vec3 e4 = vertices[base + 2].position - vertices[base+3].position;
				glm::vec2 d3 = vertices[base].tex_uv - vertices[base+3].tex_uv;
				glm::vec2 d4 = vertices[base + 2].tex_uv - vertices[base+3].tex_uv;

				glm::vec3 tangent2;
				glm::vec3 biTan2;

				float fac2 = 1.0f / (d3.x * d4.y - d4.x * d3.y);

				tangent2.x = fac2 * (d4.y * e3.x - d3.y * e4.x);
				tangent2.y = fac2 * (d4.y * e3.y - d3.y * e4.y);
				tangent2.z = fac2 * (d4.y * e3.z - d3.y * e4.z);
				tangent2 = glm::normalize(tangent2);

				biTan2.x = fac2 * (-d4.x * e3.x + d3.x * e4.x);
				biTan2.y = fac2 * (-d4.x * e3.y + d3.x * e4.y);
				biTan2.z = fac2 * (-d4.x * e3.z + d3.x * e4.z);
				biTan2 = glm::normalize(biTan2);

				vertices[base].tangent = (tangent1 + tangent2) * 0.5f;
				vertices[base].biTan = (biTan1 + biTan2) * 0.5f;
				vertices[base + 1].tangent = tangent1;
				vertices[base + 1].biTan = biTan1;
				vertices[base + 2].tangent = (tangent1 + tangent2) * 0.5f;
				vertices[base + 2].biTan = (biTan1 + biTan2) * 0.5f;
				vertices[base + 3].tangent = tangent2;
				vertices[base + 3].biTan = biTan2;

				// NOTE:: Vertex 0 and Vertex 2 are shared between both tris, so their tangents and bitangents are averaged out just in case.
			}
		}
		else {
			file.ignore(512, (int)'\n');
		}
	}
	num_verts = vertices.size();
	num_indi = indices.size();
}

void Mesh::Move(glm::vec3 dir, float dt)
{
	model = glm::translate(model, dir * PLAYER_SPEED * dt);
}

void Mesh::SetPosition(glm::vec3 pos)
{
	model = glm::translate(model, pos);
}