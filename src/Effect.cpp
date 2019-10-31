#include "Effect.h"
#include<Mesh.h>
#include<Texture.h>
#include<Hitbox.h>
#include"Light.h"
#include"Lerp.h"

void Effect::Die(std::vector<Effect*> &effect_list)
{
	for (int c = 0; c < effect_list.size(); c++) {
		if (this == effect_list.at(c)) {
			effect_list.erase(effect_list.begin() + c);
			break;
		}
	}
}

bool Effect::Cull()
{
	return life <= 0.0f;
}

Explosion::Explosion(glm::vec3 pos, std::vector<PointLight*>& l_list): Effect(MESH, MATERIAL, HITBOX, pos, LIFETIME)
{
	glow = new PointLight(pos, {1.0f, 0.8f, 0.0f}, 0.1f, 0.4f, 0.8f, L_START , Q_START);
	l_list.push_back(glow);
}

void Explosion::Update(float dt)
{
	transform.scale = lerp(glm::vec3(1.0f) * SCALE_END, glm::vec3(1.0f) * SCALE_START, life / LIFETIME);
	glow->SetIntensity(
		lerp(L_END, L_START, life / LIFETIME),
		lerp(Q_END, Q_START, life / LIFETIME)
	);

	life -= dt;
}

void Explosion::Die(std::vector<Effect*> &effect_list, std::vector<PointLight*> &l_list)
{
	for (int c = 0; c < l_list.size(); c++) {
		if (glow == l_list.at(c))
			l_list.erase(l_list.begin() + c);
	}

	Effect::Die(effect_list);
}

Mesh* Explosion::MESH = nullptr;
Material* Explosion::MATERIAL = nullptr;
Hitbox* Explosion::HITBOX = nullptr;

const float Explosion::LIFETIME = 0.3f;
const float Explosion::L_START = 0.7f;
const float Explosion::Q_START = 1.8f;
const float Explosion::L_END = 0.35f;
const float Explosion::Q_END = 0.44f;
const float Explosion::SCALE_START = 0.5f;
const float Explosion::SCALE_END = 3.0f;