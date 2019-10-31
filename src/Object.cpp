#include "Object.h"
#include<GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"Hitbox.h"
#include"Lerp.h"
#include"Light.h"

Object::Object()
{
	mesh = nullptr;
	material = nullptr;
	hitbox = nullptr;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
	phys.move = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(Mesh* me, Material* ma, Hitbox* hb)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
	phys.move = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = pos;
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
	phys.move = glm::vec3(0.0f, 0.0f, 0.0f);
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

	transform.scale = {2.0f, 2.0f, 2.0f};

	glow = new PointLight(pos, { 1.0f, 0.8f, 0.0f }, 0.0f, 0.3f, 0.5f, 1.0f, 2.6f);
}

void Bullet::Die(std::vector<Bullet*> &bull_list, std::vector<PointLight*> &l_list)
{
	parent->ReadyToFire();

	for (int c = 0; c < bull_list.size(); c++) {
		if (this == bull_list.at(c)) {
			bull_list.erase(bull_list.begin() + c);
			break;
		}
	}

	for (int l = 0; l < l_list.size(); l++) {
		if (glow == l_list.at(l)) {
			l_list.erase(l_list.begin() + l);
			break;
		}
	}
}

void Bullet::ApplyMove()
{
	glow->Move(phys.move);
	Object::ApplyMove();
}

void Bullet::Update(float dt)
{
	phys.move = dir * BULLET_SPEED * dt;
}

void Bullet::Bounce(Object* other)
{
	glm::vec3 n;

	glm::vec3 saveDir = dir;

	if (hit_x) {
		dir.x = -dir.x;
		hit_x = false;
	}
	if (hit_z) {
		dir.z = -dir.z;
		hit_z = false;
	}
	
	life--;
}

bool Bullet::Cull()
{
	return life <= 0;
}

bool Bullet::HitDetect(Object* other, float dt)
{
	Transform predict = transform;
	predict.position += phys.move;

	if (other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, predict)) {
		predict = transform;
		predict.position += glm::vec3(phys.move.x, 0.0f, 0.0f);
		if (other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, predict)) {
			hit_x = true;
		}

		predict = transform;
		predict.position += glm::vec3(0.0f, 0.0f, phys.move.z);
		if (other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, predict)) {
			hit_z = true;
		}

		for (float t = 0.0f; t <= 1.1f; t += 0.1f) {
			t = glm::min(t, 1.0f);
			Transform check = transform;
			glm::vec3 move = lerp(phys.move, glm::vec3(0.0f,0.0f,0.0f), t);

			check.position += move;
			if (!other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, check) || t >= 1.0f) {
				phys.move = move;
				break;
			}
		}

		return true;
	}
	else
		return false;
}

Tank::Tank(glm::vec3 pos, int player):Object(MESH, (player == PLAYER_1)? P1_MAT : P2_MAT, HITBOX, pos)
{
}

void Tank::Update(float dt)
{
	float t;
	float rot;
	if (anim > ANIM_SEC) {
		t = (anim - ANIM_SEC) / ANIM_SEC;
		rot = lerp(RECOIL_ANGLE, 0.0f, t);
	}
	else {
		t = anim / ANIM_SEC;
		rot = lerp(0.0f, RECOIL_ANGLE, t);
	}

	transform.rotation.x = rot;

	if (anim > 0.0f) {
		anim -= dt;
		if(anim <= 0.0f)
			glm::max(anim, 0.0f);
	}
}

void Tank::Draw(Shader* shader, std::vector<Camera*> cam)
{
	if (alive) {
		Object::Draw(shader, cam);
	}
}

Bullet* Tank::Shoot()
{
	if (canShoot && alive) {
		canShoot = false;

		glm::vec3 dir;
		dir.x = -glm::sin(glm::radians(transform.rotation.y));
		dir.y = 0.0f;
		dir.z = -glm::cos(glm::radians(transform.rotation.y));

		glm::vec3 spawnPos = transform.position + dir * (1.0f);

		anim = ANIM_SEC * 2;

		return new Bullet(spawnPos, dir, this);
	}
	
	return nullptr;
}

bool Tank::HitDetect(Object* other, float dt)
{
	Transform predict = transform;
	predict.position += phys.move;

	if (other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, predict)) {

		for (float t = 0.0f; t < 1.1f; t += 0.1f) {
			t = glm::min(t, 1.0f);
			Transform check = transform;
			glm::vec3 move = lerp(phys.move, glm::vec3(0.0f, 0.0f, 0.0f), t);

			check.position += move;
			if (!other->hitbox->HitDetect(other->GetTransform(), (SphereHitbox*)this->hitbox, check) || t >= 1.0f) {
				phys.move = move;
				break;
			}
		}

		return true;
	}
	else
		return false;
}

Mesh* Tank::MESH = nullptr;
Material* Tank::P1_MAT = nullptr;
Material* Tank::P2_MAT = nullptr;
Hitbox* Tank::HITBOX = nullptr;
const float Tank::ANIM_SEC = 0.1f;
const float Tank::RECOIL_ANGLE = 10.0f;

Mesh* Bullet::MESH = nullptr;
Material* Bullet::MATERIAL = nullptr;
Hitbox* Bullet::HITBOX = nullptr;
