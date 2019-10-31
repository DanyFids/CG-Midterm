#include "PlayScenes.h"
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<GLM/glm.hpp>
#include <iostream>
#include <typeinfo>

#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"Camera.h"
#include"Light.h"
#include"Object.h"
#include"Mesh.h"
#include"Constants.h"
#include"Test_Primitives.h"
#include"Hitbox.h"
#include"Effect.h"


GameScene::GameScene()
{
	LoadScene();
}

void GameScene::InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		ControllerInput(GLFW_JOYSTICK_1, PLAYER_1, dt);
	}
	else {
		KeyboardInput(window, mousePos, PLAYER_1, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3_pressed) {
		if (debug) {
			if (disp_depth != lights.size() - 1)
				disp_depth++;
			else
				debug = false;
		}
		else {
			if (lights.size() > 0) {
				debug = true;
				disp_depth = 0;
			}
		}

		f3_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE)
		f3_pressed = false;
}

void GameScene::Update(float dt)
{
	for (int p = 0; p < players.size(); p++) {
		players[p]->Update(dt);

		for (int o = 0; o < players.size(); o++) {
			if (players[p] != players[o])
				players[p]->HitDetect(players[o], dt);
		}

		for (int t = 1; t < terrain.size(); t++) {
			players[p]->HitDetect(terrain[t], dt);
		}
	}

	for (int c = 0; c < bullets.size(); c++) {
		bullets.at(c)->Update(dt);

		if (bullets.at(c)->Cull()) {
			bullets.at(c)->Die(bullets, lights);
			c--;
			continue;
		}

		bool kill = false;
		for (int p = 0; p < players.size(); p++) {
			if (players[p]->alive && bullets.at(c)->HitDetect(players.at(p), dt)) {
				kill = true;
				players[p]->alive = false;
				endRound = true;
			}
		}

		for (int b = 0; b < bullets.size(); b++) {
			if(bullets[c] != bullets[b])
				if (bullets[c]->HitDetect(bullets[b], dt)) {
					kill = true;
					bullets[b]->Die(bullets, lights);
					if (c > b) {
						c--;
					}
					break;
				}
		}

		if (kill) {
			bullets.at(c)->Die(bullets, lights);
			c--;

			if (bullets.size() <= 0) break;
			else continue;
		}

		for (int t = 0; t < terrain.size(); t++) {
			if (bullets.at(c)->HitDetect(terrain.at(t), dt)) {
				bullets.at(c)->Bounce(terrain.at(t));
			}
		}

		bullets.at(c)->ApplyMove();
	}

	for (int p = 0; p < players.size(); p++) {
		players[p]->ApplyMove();
	}

	if (endRound) {
		endTimer -= dt;
		if (endTimer <= 0.0f)
			Reset();
	}
}

void GameScene::Draw()
{
	glCullFace(GL_FRONT);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, sun->GetFrameBuffer());
	glClear(GL_DEPTH_BUFFER_BIT);
	sun->SetupDepthShader(sunShader);
	RenderScene(sunShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, sun->GetDepthMap());

	for (int l = 0; l < lights.size(); l++) {
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, lights[l]->GetFrameBuffer());
		glClear(GL_DEPTH_BUFFER_BIT);
		lights[l]->SetupDepthShader(depthShader);
		RenderScene(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		glActiveTexture(GL_TEXTURE4 + l);
		glBindTexture(GL_TEXTURE_CUBE_MAP, lights[l]->GetDepthMap());
	}
	glCullFace(GL_BACK);

	sun->SetupLight(shaderObj);
	for (int c = 0; c < lights.size(); c++) {
		lights[c]->SetupLight(shaderObj, c);
	}
	shaderObj->SetI("num_lights", lights.size());

	for (int c = 0; c < Cam.size(); c++) {
		Cam[c]->SetupCam(shaderObj);

		RenderScene(shaderObj);

		Cam[c]->SetupCam(bulShader);
		for (int c = 0; c < bullets.size(); c++) {
			bullets[c]->Draw(bulShader, Cam);
		}
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void GameScene::LoadScene()
{
	shaderObj = new Shader("Shaders/Basic_Shader - NM.vert", "Shaders/Basic_Shader - NM.frag");
	bulShader = new Shader("Shaders/bullet.vert", "Shaders/bullet.frag");
	depthShader = new Shader("Shaders/depth_shader.vert", "Shaders/depth_shader.frag", "Shaders/depthGeo.glsl");
	sunShader = new Shader("Shaders/sunDepth.vert", "Shaders/sunDepth.frag");

	Material* DiceTex = new Material("dice-texture.png", "d6-normal.png");
	Material* D20Tex = new Material("d20-texture.png");
	Material* TankP1Tex = new Material("RedCamo.png");
	Material* TankP2Tex = new Material("BlueCamo.png");
	//Material* SwordTex = new Material("sword-texture.png", "sword-norm.png");
	Material* bulletTex = new Material("yellow.png");
	Material* defaultTex = new Material("default-texture.png");
	defaultTex->shine = 512;

	sun = new DirectionalLight(glm::normalize(glm::vec3(5.0f, 25.0f, 0.5f)), { 1.0f, 1.0f, 1.0f }, 0.2f, 0.3f, 0.4f);
	//lights.push_back(new PointLight({ 0.5f, 30.0f, 0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, 0.3f, 0.5f, 1.0f, 0.014f, 0.0007f));
	lights.push_back(new PointLight({ -4.0f, 3.0f, 4.0f }, { 1.0f, 1.0f, 1.0f }, 0.1f, 0.5f, 1.0f, 0.07f, 0.017f));

	Mesh* Square = new Mesh("d6.obj");
	Mesh* d20 = new Mesh("d20.obj");
	Mesh* TankM = new Mesh("Tank_c.obj");
	Mesh* BulletM = new Mesh("Pellet.obj");

	Hitbox* basicCubeHB = new CubeHitbox(1.0f,1.0f,1.0f);
	Hitbox* sphereHB = new SphereHitbox(0.7f);
	Hitbox* pelletHB = new SphereHitbox(0.05f);

	Tank::MESH = TankM;
	Tank::P1_MAT = TankP1Tex;
	Tank::P2_MAT = TankP2Tex;
	Tank::HITBOX = sphereHB;

	Bullet::MESH = BulletM;
	Bullet::MATERIAL = bulletTex;
	Bullet::HITBOX = pelletHB;

	Explosion::MESH = ;

	players.push_back(new Tank({ 0.0f, 0.3f, 0.0f }));
	players[PLAYER_1]->Scale({0.75f, 0.75f, 0.75f});

	players.push_back(new Tank({ -2.0f, 0.3f, 0.0f }, PLAYER_2));
	players[PLAYER_2]->Scale({ 0.75f, 0.75f, 0.75f });

	Object* floor = new Object(Square, defaultTex, basicCubeHB);
	floor->Move({ 0.0f, -0.75f, 0.0f });
	floor->Scale({ 20.0f, 0.5f, 20.0f });

	Object* top_wall = new Object(Square, defaultTex, basicCubeHB);
	Object* right_wall = new Object(Square, defaultTex, basicCubeHB);
	Object* left_wall = new Object(Square, defaultTex, basicCubeHB);
	Object* bot_wall = new Object(Square, defaultTex, basicCubeHB);

	top_wall->Move(glm::vec3(0.0f, 0.5f, 5.0f));
	right_wall->Move(glm::vec3(7.5f, 0.5f, -0.5f));
	left_wall->Move(glm::vec3(-7.5f, 0.5f, -0.5f));
	bot_wall->Move(glm::vec3(0.0f, 0.5f, -5.5f));

	top_wall->Scale(glm::vec3(20.0f, 1.0f, 1.0f));
	right_wall->Scale(glm::vec3(1.0f, 1.0f, 10.0f));
	left_wall->Scale(glm::vec3(1.0f, 1.0f, 10.0f));
	bot_wall->Scale(glm::vec3(20.0f, 1.0f, 1.0f));

	terrain = {
		floor,
		top_wall,
		right_wall,
		left_wall,
		bot_wall
	};

	Cam = {
		new Camera(glm::vec3(0.0f, 15.0f, -1.0f), glm::vec3(0,0,0), glm::vec4(0,0, SCREEN_WIDTH, SCREEN_HEIGHT))
	};

	this->Reset();
}

void GameScene::Reset()
{
	players[PLAYER_1]->SetPosition({ 3.0f, 0.0f, -0.5f });
	players[PLAYER_2]->SetPosition({-3.0f, 0.0f, -0.5f});

	players[PLAYER_1]->SetRotation({ 0.0f, 90.0f, 0.0f });
	players[PLAYER_2]->SetRotation({ 0.0f,-90.0f,0.0f });

	players[PLAYER_1]->alive = true;
	players[PLAYER_2]->alive = true;

	players[PLAYER_1]->ReadyToFire();
	players[PLAYER_2]->ReadyToFire();

	endRound = false;
	endTimer = END_TIME;

	while (bullets.size() > 0) {
		bullets[0]->Die(bullets, lights);
	}
}