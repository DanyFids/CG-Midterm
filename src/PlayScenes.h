#pragma once
#include "Scene.h"

class Light;

class GameScene: public PlayScene {
	Shader* shaderObj;
	Shader* depthShader;
	Shader* sunShader;

	bool debug = false;
	bool f3_pressed;
	int disp_depth = 0;

public:
	GameScene();

	// Inherited via Scene
	virtual void InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt) override;
	virtual void Update(float dt) override;
	virtual void Draw() override;
	virtual void LoadScene() override;
};