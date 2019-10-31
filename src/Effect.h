#include<Object.h>

class Effect : public Object {
protected:
	float life, lifeTime;

public:
	Effect(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos, float lt) :Object(me, ma, hb, pos), life(lt), lifeTime(lt) {  }

	virtual void Die(std::vector<Effect*> &effect_list);
	virtual bool Cull();
};

class Explosion :public Effect {
	const float LIFETIME = 1.0f;
	const float L_START = 0.7f;
	const float Q_START = 1.8f;

	const float L_END = 0.35f;
	const float Q_END = 0.44f;

	const float SCALE_START = 1.0f;
	const float SCALE_END = 3.0f;

public:
	PointLight* glow;

	static Mesh* MESH;
	static Material* MATERIAL;
	static Hitbox* HITBOX;

	Explosion(glm::vec3 pos);
	
	virtual void Update(float dt);

	virtual void Die(std::vector<Effect*> &effect_list, std::vector<PointLight*> &l_list);

};