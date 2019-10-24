#pragma once
#include<GLM/glm.hpp>

struct Transform;

class CubeHitbox;
class SphereHitbox;
class CapsuleHitbox;

class Hitbox {

public:
	virtual bool HitDetect(Transform t, CubeHitbox * other, Transform oT) = 0;
	virtual bool HitDetect(Transform t, SphereHitbox* other, Transform oT) = 0;
	virtual bool HitDetect(Transform t, CapsuleHitbox* other, Transform oT) = 0;
};

class CubeHitbox : public Hitbox {
	glm::vec3 dim;
public:
	CubeHitbox(float w, float h, float d);

	glm::vec3 GetDim() {return dim;}

	virtual bool HitDetect(Transform t, CubeHitbox* other, Transform oT);
	virtual bool HitDetect(Transform t, SphereHitbox* other, Transform oT);
	virtual bool HitDetect(Transform t, CapsuleHitbox* other, Transform oT);
};

class SphereHitbox : public Hitbox {
	float radius;
public:
	SphereHitbox(float r) :radius(r) {};

	float GetRadius() { return radius; }

	virtual bool HitDetect(Transform t, CubeHitbox* other, Transform oT);
	virtual bool HitDetect(Transform t, SphereHitbox* other, Transform oT);
	virtual bool HitDetect(Transform t, CapsuleHitbox* other, Transform oT);
};