#include "Camera.h"
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include"Constants.h"

Camera::Camera(glm::vec3 pos, glm::vec4 s_dim)
{
	position = pos;
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::normalize(target - position);
	glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(u, direction));
	up = glm::normalize(glm::cross(direction, right));

	pitch = direction.y * 90;
	yaw = (direction.x + direction.z) * 90;

	lookAt = glm::lookAt(position, target, u);
	project = glm::perspective(glm::radians(45.0f), (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 50.0f); // projection Matrix
	screen_dim = s_dim;
	view_cor = glm::mat4(1.0f);
	view_cor = glm::scale(view_cor, glm::vec3((float)SCREEN_WIDTH / s_dim.z, 1.0f, 1.0f));

}

Camera::Camera(glm::vec3 pos, glm::vec3 targ, glm::vec4 s_dim)
{
	position = pos;
	target = targ;
	direction = glm::normalize(target - position);
	glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(direction, u));
	up = glm::normalize(glm::cross(direction, right));

	pitch = direction.y * 90;
	yaw = (direction.x + direction.z) * 90;

	lookAt = glm::lookAt(position, target, u);
	project = glm::perspective(glm::radians(45.0f), (float)(s_dim.z / s_dim.w), 0.1f, 50.0f); // projection Matrix
	screen_dim = s_dim; // Set Screen Position

	view_cor = glm::mat4(1.0f);
	view_cor = glm::translate(view_cor, glm::vec3(-(SCREEN_WIDTH - s_dim.z) / 2.0f, 0.0f, 0.0f));
	view_cor = glm::scale(view_cor, glm::vec3((float)SCREEN_WIDTH / s_dim.z, 1.0f, 1.0f));
}

void Camera::Move(glm::vec3 dir, float dt)
{
	position = position + (dir * speed * dt);

	UpdateCam();
}

void Camera::Rotate(glm::vec2 dir)
{
	pitch += dir.y;
	yaw += dir.x;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if(pitch < -89.0f){
		pitch = -89.0f;
	}

	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	UpdateCam();
}

void Camera::SetPosition(glm::vec3 p)
{
	position = p;

	UpdateCam();
}

void Camera::SetTarget(glm::vec3 t)
{
	target = t;

	direction = glm::normalize(target - position);
	UpdateCam();
}

void Camera::Spin(glm::vec2 dir)
{
	yaw += dir.x;
	pitch -= dir.y;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < 0.0f) {
		pitch = 0.0f;
	}

	float camX = sin(glm::radians(yaw)) * radius;
	float camZ = cos(glm::radians(yaw)) * radius;
	float camY = sin(glm::radians(pitch)) * radius;

	glm::vec3 hoverPos = glm::vec3(camX, camY, camZ);

	position = hoverPos + target;
	direction = glm::normalize(target - position);
	UpdateCam();
}

void Camera::UpdateCam()
{
	glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(direction, u));
	up = glm::normalize(glm::cross(right, direction));

	lookAt = glm::lookAt(position, position + direction, up);
}

void Camera::SetupCam(Shader* shader)
{
	glViewport(screen_dim.x, screen_dim.y, screen_dim.z, screen_dim.w);

	unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(lookAt));
	unsigned int projLoc = glGetUniformLocation(shader->ID, "project");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(project));
	unsigned int corrLoc = glGetUniformLocation(shader->ID, "corr");
	glUniformMatrix4fv(corrLoc, 1, GL_FALSE, glm::value_ptr(view_cor));

	//shader->SetVec3("lightPos", position);
	shader->SetVec3("viewPos", position);
}

void Camera::TogglePerspective()
{
	view_p = !view_p;
	if (view_p) {
		project = glm::perspective(glm::radians(45.0f), (float)(screen_dim.z / screen_dim.w), 0.1f, 50.0f);
	}
	else {
		project = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 50.0f);
	}
}

glm::mat4 Camera::GetLookAt()
{
	return lookAt;
}

glm::mat4 Camera::GetProject()
{
	return project;
}

glm::vec3 Camera::GetDirection()
{
	return direction;
}

glm::vec3 Camera::GetRight()
{
	return right;
}

glm::vec3 Camera::GetUp()
{
	return up;
}

float Camera::GetRotateSpeed()
{
	return rotSpeed;
}

