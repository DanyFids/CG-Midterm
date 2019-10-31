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

Explosion::Explosion(glm::vec3 pos): Effect(MESH, MATERIAL, HITBOX, pos, LIFETIME)
{
	glow = new PointLight(pos, {}, 0.1f, 0.4f, 0.8f, L_START , Q_START);
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
