#include "Hitbox.h"
#include "Object.h"

CubeHitbox::CubeHitbox(float w, float h, float d):dim(glm::vec3(w,h,d))
{
}

bool CubeHitbox::HitDetect(Transform t, CubeHitbox* other, Transform oT) // AABB
{
	float halfW = GetDim().x / 2 * t.scale.x;
	float halfH = GetDim().y / 2 * t.scale.y;
	float halfD = GetDim().z / 2 * t.scale.z;

	float ohw = other->GetDim().x / 2 * oT.scale.x;
	float ohh = other->GetDim().y / 2 * oT.scale.y;
	float ohd = other->GetDim().z / 2 * oT.scale.z;

	if (t.position.x - halfW < oT.position.x + ohw && t.position.x + halfW > oT.position.x - ohw &&
		t.position.y - halfH < oT.position.y + ohh && t.position.y + halfH > oT.position.y - ohh &&
		t.position.z - halfD < oT.position.z + ohd && t.position.z + halfD > oT.position.z - ohd) {
		return true;
	}else{ 
		return false; 
	}
}

bool CubeHitbox::HitDetect(Transform t, SphereHitbox* other, Transform oT)
{
	float ohw = dim.x / 2 * t.scale.x;
	float ohh = dim.y / 2 * t.scale.y;
	float ohd = dim.z / 2 * t.scale.z;

	glm::vec3 closestPoint = glm::vec3(
		glm::max(t.position.x - ohw, glm::min(oT.position.x, t.position.x + ohw)),
		glm::max(t.position.y - ohh, glm::min(oT.position.y, t.position.y + ohh)),
		glm::max(t.position.z - ohd, glm::min(oT.position.z, t.position.z + ohd))
	);

	float distance = glm::distance(oT.position, closestPoint);

	if (distance < other->GetRadius())
		return true;

	return false;
}

bool CubeHitbox::HitDetect(Transform t, CapsuleHitbox* other, Transform oT)
{
	return false;
}

/********************************************************************
*						SPHERE COLLISION
********************************************************************/

bool SphereHitbox::HitDetect(Transform t, CubeHitbox* other, Transform oT)
{
	float ohw = other->GetDim().x / 2 * oT.scale.x;
	float ohh = other->GetDim().y / 2 * oT.scale.y;
	float ohd = other->GetDim().z / 2 * oT.scale.z;

	glm::vec3 closestPoint = glm::vec3(
		glm::max(oT.position.x - ohw, glm::min(t.position.x, oT.position.x + ohw)),
		glm::max(oT.position.y - ohh, glm::min(t.position.y, oT.position.y + ohh)),
		glm::max(oT.position.z - ohd, glm::min(t.position.z, oT.position.z + ohd))
	);

	float distance = glm::distance(t.position, closestPoint);
	
	if (distance < radius)
		return true;

	return false;
}

bool SphereHitbox::HitDetect(Transform t, SphereHitbox* other, Transform oT)
{
	float distance = glm::distance(t.position, oT.position);
	float maxDistance = radius * (t.scale.x + t.scale.y + t.scale.z) / 3 + other->GetRadius() * (oT.scale.x + oT.scale.y + oT.scale.z) / 3;
	
	if (distance < maxDistance)
		return true;
	return false;
}

bool SphereHitbox::HitDetect(Transform t, CapsuleHitbox* other, Transform oT)
{
	return false;
}
