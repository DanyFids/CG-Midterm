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
	float offsetX = ((float)mousePos.x - m_lastX) * MOUSE_SENSITIVITY;
	float offsetY = (m_lastY - (float)mousePos.y) * MOUSE_SENSITIVITY;
	m_lastX = (float)mousePos.x;
	m_lastY = (float)mousePos.y;

	if ((offsetX != 0.0f || offsetY != 0.0f) && !firstMouse) {
		Cam[player]->Rotate(glm::vec2(offsetX, offsetY));
	}
	else if ((offsetX != 0.0f || offsetY != 0.0f) && firstMouse) {
		firstMouse = false;
	}

	glm::vec3 m = glm::vec3(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		m -= glm::vec3(0.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		m += glm::vec3(0.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		m -= glm::vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		m += glm::vec3(1.0f, 0.0f, 0.0f);
	if (m.x != 0.0f || m.y != 0.0f || m.z != 0.0f)
		players[player]->Move(m * PLAYER_SPEED * dt);

	glm::vec3 t = glm::vec3(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		t += Cam[player]->GetDirection();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		t -= Cam[player]->GetDirection();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		t -= Cam[player]->GetRight();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		t += Cam[player]->GetRight();
	if (t.x != 0.0f || t.y != 0.0f || t.z != 0.0f)
		Cam[player]->Move(glm::normalize(t), dt);
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

	for (int t = 0; t < terrain.size(); t++) {
		terrain[t]->Draw(shader, Cam);
	}
}
