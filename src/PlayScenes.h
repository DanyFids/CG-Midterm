#pragma once
#include "Scene.h"

class Light;
class Effect;
class Material;

class GameScene: public PlayScene {
	Shader* shaderObj;
	Shader* depthShader;
	Shader* sunShader;
	Shader* bulShader;

	Material* RedPointMat;
	Material* BluPointMat;

	int redScore = 0;
	int bluScore = 0;

	bool debug = false;
	bool f3_pressed;
	int disp_depth = 0;

	const float END_TIME = 2.0f;

	float endTimer = END_TIME;
	bool endRound;

	std::vector<Effect*> effects;
public:
	GameScene();

	// Inherited via Scene
	virtual void InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt) override;
	virtual void Update(float dt) override;
	virtual void Draw() override;
	virtual void LoadScene() override;
	virtual void Reset();
};