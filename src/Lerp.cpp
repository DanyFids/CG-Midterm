#include "Lerp.h"

float lerp(float f1, float f2, float t) {
	return(1.0f - t) * f1 + t * f2;
}

glm::vec3 lerp(glm::vec3 p1, glm::vec3 p2, float t){
	glm::vec3 ret;

	ret.x = lerp(p1.x, p2.x, t);
	ret.y = lerp(p1.y, p2.y, t);
	ret.z = lerp(p1.z, p2.z, t);

	return ret;
}