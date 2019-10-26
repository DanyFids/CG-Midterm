#include "Scene.h"

#include"Camera.h"
#include"Mesh.h"
#include"Object.h"
#include"Light.h"
#include "Constants.h"

void MenuScene::KeyboardInput(GLFWwindow* window, glm::vec2 mousePos, int player, float dt)
{
}

void MenuScene::ControllerInput(unsigned int controller, int player, float dt)
{
}

void MenuScene::Update(float dt)
{
}

void MenuScene::Draw()
{
}

void MenuScene::LoadScene()
{
}

void MenuScene::InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt)
{
}

void PlayScene::KeyboardInput(GLFWwindow* window, glm::vec2 mousePos, int player, float dt)
{
	// PLAYER 1
	glm::vec3 t = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		rotate = false;
		t.x += glm::cos(glm::radians(players[PLAYER_1]->GetTransform().rotation.y));
		t.y = 0.0f;
		t.z -= glm::sin(glm::radians(players[PLAYER_1]->GetTransform().rotation.y));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		rotate = false;
		t.x -= glm::cos(glm::radians(players[PLAYER_1]->GetTransform().rotation.y));
		t.y = 0.0f;
		t.z += glm::sin(glm::radians(players[PLAYER_1]->GetTransform().rotation.y));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rotate = true;
		rotation.y += 20 * PLAYER_SPEED * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotate = true;
		rotation.y -= 20 * PLAYER_SPEED * dt;
	}

	if (t.x != 0.0f || t.y != 0.0f || t.z != 0.0f) {
		if(!rotate)
			players[PLAYER_1]->Move(t * PLAYER_SPEED * dt);
	}
	if (rotate)
		players[PLAYER_1]->Rotate(rotation);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !p1_shoot) {
		players[PLAYER_1]->Shoot(bullets);
		p1_shoot = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		p1_shoot = false;
	}

	// PLAYER 2
	glm::vec3 m = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation2 = glm::vec3(0.0f);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		rotate2 = false;
		m.x += glm::cos(glm::radians(players[PLAYER_2]->GetTransform().rotation.y));
		m.y = 0.0f;
		m.z -= glm::sin(glm::radians(players[PLAYER_2]->GetTransform().rotation.y));
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		rotate2 = false;
		m.x -= glm::cos(glm::radians(players[PLAYER_2]->GetTransform().rotation.y));
		m.y = 0.0f;
		m.z += glm::sin(glm::radians(players[PLAYER_2]->GetTransform().rotation.y));
	}	

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		rotate2 = true;
		rotation2.y += 20 * PLAYER_SPEED * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		rotate2 = true;
		rotation2.y -= 20 * PLAYER_SPEED * dt;
	}

	if (m.x != 0.0f || m.y != 0.0f || m.z != 0.0f) {
		if (!rotate2)
			players[PLAYER_2]->Move(m * PLAYER_SPEED * dt);
	}

	if (rotate2) 
		players[PLAYER_2]->Rotate(rotation2);

	rotate = false;
	rotate2 = false;

	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS && !p2_shoot) {
		players[PLAYER_2]->Shoot(bullets);
		p2_shoot = true;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE) {
		p2_shoot = false;
	}
}

void PlayScene::ControllerInput(unsigned int controller, int player, float dt)
{
	GLFWgamepadstate state;
	if (glfwGetGamepadState(controller, &state)) {
		glm::vec2 rot = glm::vec2(0.0f, 0.0f);
		if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > 0.2 || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -0.2) {
			rot.y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
		}
		else {
			rot.y = 0.0f;
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > 0.2 || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] < -0.2) {
			rot.x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		}
		else {
			rot.x = 0.0f;
		}
		Cam[player]->Spin(rot * Cam[player]->GetRotateSpeed() * dt);

		glm::vec3 t = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 camF = Cam[player]->GetDirection();
		glm::vec3 camR = Cam[player]->GetRight();
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.2 || state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.2)
			t -= glm::normalize(glm::vec3(camF.x, 0.0f, camF.z)) * state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.2 || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.2)
			t += glm::normalize(glm::vec3(camR.x, 0.0f, camR.z)) * state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		if (t.x != 0.0f || t.y != 0.0f || t.z != 0.0f) {
			players[player]->Move(glm::normalize(t) * 10.f * dt);
			Cam[player]->Move(glm::normalize(t), dt); 
			Cam[player]->SetTarget(players[player]->GetPosition());
		}
	}
}

void PlayScene::RenderScene(Shader* shader)
{
	for (int c = 0; c < players.size(); c++) {
		players[c]->Draw(shader, Cam);
	}

	for (int c = 0; c < bullets.size(); c++) {
		bullets[c]->Draw(shader, Cam);
	}

	for (int t = 0; t < terrain.size(); t++) {
		terrain[t]->Draw(shader, Cam);
	}
}
