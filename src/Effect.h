#include"Object.h"

class Effect : public Object {
protected:
	float life, lifeTime;

public:
	Effect(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos, float lt) :Object(me, ma, hb, pos), life(lt), lifeTime(lt) {  }

	virtual void Die(std::vector<Effect*> &effect_list);
	virtual bool Cull();
};

class Explosion :public Effect {
	static const float LIFETIME;
	static const float L_START;
	static const float Q_START;

	static const float L_END;
	static const float Q_END;

	static const float SCALE_START;
	static const float SCALE_END;

public:
	PointLight* glow;

	static Mesh* MESH;
	static Material* MATERIAL;
	static Hitbox* HITBOX;

	Explosion(glm::vec3 pos, std::vector<PointLight*> &l_list);
	
	virtual void Update(float dt);

	virtual void Die(std::vector<Effect*> &effect_list, std::vector<PointLight*> &l_list);

};