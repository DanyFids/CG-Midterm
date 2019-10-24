#include "Object.h"
#include<GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"Hitbox.h"
#include"Lerp.h"

Object::Object()
{
	mesh = nullptr;
	material = nullptr;
	hitbox = nullptr;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Object::Object(Mesh* me, Material* ma, Hitbox* hb)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Object::Object(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = pos;
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

void Object::Update(float dt)
{
}

void Object::Draw(Shader* shader, std::vector<Camera*> cams)
{
	shader->Use();
	shader->SetI("material.diffuse", 0);
	shader->SetI("material.normal", 1);
	shader->SetI("material.specular", 2);

	glm::mat4 model = transform.GetWorldTransform();

	unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat3 normMat = glm::mat3(glm::transpose(glm::inverse(model)));

	shader->SetMat3("normMat", normMat);

	shader->SetF("material.shine", material->shine);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material->DIFF);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material->NORM);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material->SPEC);

	mesh->Draw(shader, cams);
}

void Object::Rotate(glm::vec3 rot) {
	transform.rotation += rot;
}

void Object::Move(glm::vec3 dir)
{
	transform.position += dir;
}

void Object::Scale(glm::vec3 scl)
{
	transform.scale = scl;
}

const float Bullet::BULLET_SPEED = 10.0f;

Bullet::Bullet(glm::vec3 pos, glm::vec3 d, Tank* p):Object(MESH, MATERIAL, HITBOX, pos)
{
	dir = d;
	parent = p;
}

void Bullet::Die(std::vector<Bullet*> &bull_list)
{
	parent->ReadyToFire();

	for (int c = 0; c < bull_list.size(); c++) {
		if (this == bull_list.at(c)) {
			bull_list.erase(bull_list.begin() + c);
		}
	}
}

void Bullet::Update(float dt)
{
	Move(dir * BULLET_SPEED * dt);
}

void Bullet::Bounce(Object* other)
{
	glm::vec3 n;


	//n = glm::normalize(n);
	//dir = dir - 2.0f*(glm::dot(dir, n)*n);
	dir = -dir;
	life--;
}

bool Bullet::Cull()
{
	return life <= 0;
}

bool Bullet::HitDetect(Object* other, float dt)
{
	Transform predict = transform;
	predict.position += dir * BULLET_SPEED * dt;

	if (other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, predict)) {
		for (float t = 1.0f; t >= 0.0f; t -= 0.1f) {
			Transform check = transform;
			glm::vec3 move = lerp(dir*BULLET_SPEED*dt, glm::vec3(0.0f,0.0f,0.0f), t);

			check.position += move;
			if (!other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, predict)) {
				dir = move / (BULLET_SPEED * dt);
				break;
			}
		}
		return true;
	}
	else
		return false;
}

Tank::Tank(glm::vec3 pos):Object(MESH, MATERIAL, HITBOX, pos)
{
}

void Tank::Shoot(std::vector<Bullet*>& bul_list)
{
	if (canShoot) {
		canShoot = false;

		glm::vec3 dir;
		dir.x = glm::cos(glm::radians(transform.rotation.y));
		dir.y = 0.0f;
		dir.z = glm::sin(glm::radians(transform.rotation.y));

		glm::vec3 spawnPos = transform.position + dir * (1.0f);

		bul_list.push_back(new Bullet(spawnPos, dir, this));
	}
}

bool Tank::HitDetect(Object* other, float dt)
{
	return false;
}

Mesh* Tank::MESH = nullptr;
Material* Tank::MATERIAL = nullptr;
Hitbox* Tank::HITBOX = nullptr;

Mesh* Bullet::MESH = nullptr;
Material* Bullet::MATERIAL = nullptr;
Hitbox* Bullet::HITBOX = nullptr;
