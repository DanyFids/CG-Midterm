#pragma once
#include<GLM/glm.hpp>
class Shader;

class Camera {
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec4 screen_dim;
	glm::mat4 lookAt;
	glm::mat4 project;
	glm::mat4 view_cor;

	float radius = 10.0f;
	float speed = 10.0f;
	float rotSpeed = 80.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	bool view_p = true;

public:
	Camera(glm::vec3 pos, glm::vec4 screen_dim);
	Camera(glm::vec3 pos, glm::vec3 targ, glm::vec4 screen_dim);

	void Move(glm::vec3 dir, float dt);
	void Rotate(glm::vec2 dir);
	void SetPosition(glm::vec3 p);
	void SetTarget(glm::vec3 t);
	void Spin(glm::vec2 dir);
	void UpdateCam();
	void SetupCam(Shader*);

	void TogglePerspective();

	// Getters
	glm::mat4 GetLookAt();
	glm::mat4 GetProject();
	glm::vec3 GetDirection();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	float GetRotateSpeed();
};