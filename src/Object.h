#pragma once
#include<GLM/glm.hpp>
#include<GLM/gtc/matrix_transform.hpp>
#include<GLM/gtc/quaternion.hpp>
#include<vector>

#include"Constants.h"

class Camera;
class Material;
class Mesh;
class Shader;
class Hitbox;

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 GetWorldTransform() const {
		glm::quat qPitch = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat qYaw = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat qRoll = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::quat rot = qYaw * qPitch * qRoll;

		return
			glm::translate(glm::mat4(1.0f), position) *
			glm::mat4_cast(rot) *
			glm::scale(glm::mat4(1.0f), scale)
			;
	}

	glm::mat4 GetRotScale() const {
		return
			glm::mat4_cast(glm::quat(glm::radians(rotation))) *
			glm::scale(glm::mat4(1.0f), scale);
	}
};

struct PhysicsBody {
	glm::vec3 move;
};

class Object {
protected:
	Mesh* mesh;
	Material* material;
	Transform transform;

public:
	Hitbox* hitbox;
	PhysicsBody phys;

	Object();
	Object(Mesh* me, Material* ma, Hitbox* hb);
	Object(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos);

	virtual void Update(float dt);
	virtual void Draw(Shader* shader, std::vector<Camera*> cam);
	void Rotate(glm::vec3 rot);
	void Rotate(float tht, glm::vec3 dir);
	void Move(glm::vec3 dir);
	virtual void ApplyMove() { transform.position += phys.move; phys.move = glm::vec3(0.0f, 0.0f, 0.0f); };
	void Scale(glm::vec3 scl);

	virtual bool HitDetect(Object* other, float dt) { return false; };

	glm::vec3 GetPosition() { return transform.position; };
	void SetPosition(glm::vec3 pos) { transform.position = pos; }
	void SetRotation(glm::vec3 rot) { transform.rotation = rot; }
	Transform GetTransform() { return transform; }
};

class Tank;
class PointLight;

class Bullet : public Object {
	static const float BULLET_SPEED;

	bool hit_z = false;
	bool hit_x = false;

	Tank* parent;
	glm::vec3 dir;
	int life = 3;

	PointLight* glow;
public:
	static Mesh* MESH;
	static Material* MATERIAL;
	static Hitbox* HITBOX;

	Bullet(glm::vec3 pos, glm::vec3 d, Tank* p);

	void Die(std::vector<Bullet*> &bul_list, std::vector<PointLight*> &l_list);
	PointLight* GetGlow() { return glow; }
	virtual void ApplyMove();
	virtual void Update(float dt);
	void Bounce(Object* other);
	bool Cull();
	virtual bool HitDetect(Object* other, float dt);
};

class Tank :public Object {
	bool canShoot = true;
public:
	static Mesh* MESH;
	static Material* P1_MAT;
	static Material* P2_MAT;
	static Hitbox* HITBOX;

	static const float ANIM_SEC;
	static const float RECOIL_ANGLE;

	bool alive = true;

	float anim = 0.0f;

	Tank(glm::vec3 pos, int player = PLAYER_1);

	virtual void Update(float dt);
	virtual void Draw(Shader* shader, std::vector<Camera*> cam);
	void ReadyToFire() { canShoot = true; }
	Bullet* Shoot();
	virtual bool HitDetect(Object* other, float dt);
};