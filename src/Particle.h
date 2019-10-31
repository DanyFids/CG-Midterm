#include<GLM/glm.hpp>

class Mesh;
class Material;

class Particle {
protected:
	Mesh* Quad;
	Material* Tex;
	glm::vec3 position, velocity;
	float life;

public:
	Particle(glm::vec3 pos, glm::vec3 vel, float l) :position(pos), velocity(vel), life(l) {};

	virtual void Update(float dt) = 0;
};