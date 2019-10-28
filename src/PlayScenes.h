#pragma once
#include "Scene.h"

class Light;

class GameScene: public PlayScene {
	Shader* shaderObj;
	Shader* depthShader;
	Shader* sunShader;
	Shader* bulShader;

	bool debug = false;
	bool f3_pressed;
	int disp_depth = 0;

	const float END_TIME = 2.0f;

	float endTimer = END_TIME;
	bool endRound;
public:
	GameScene();

	// Inherited via Scene
	virtual void InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt) override;
	virtual void Update(float dt) override;
	virtual void Draw() override;
	virtual void LoadScene() override;
	virtual void Reset();
};