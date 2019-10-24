#include "Game.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <functional>

#include"SceneManager.h"
#include "Constants.h"
#include"PlayScenes.h"
#include "Camera.h"
#include "Object.h"

struct UpdateBehaviour {
	std::function<void(entt::entity e, float dt)> Function;
};

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

glm::vec2 mousePos = glm::vec2(400, 300);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mousePos = glm::vec2(xpos, ypos);
}

Game::Game() :
	window(nullptr),
	windowTitle("Glade-iators"),
	myClearColor(glm::vec4(0.1f, 0.7f, 0.5f, 1.0f))
{ }


Game::~Game()
{
}

void Game::Run()
{
	Initialize();
	InitImGui();
	LoadContent();
	static float prevFrame = glfwGetTime();
	// Run as long as the window is open
	while (!glfwWindowShouldClose(window)) {
		// Poll for events from windows
		// clicks, key presses, closing, all that
		glfwPollEvents();
		float thisFrame = glfwGetTime();
		float deltaTime = thisFrame - prevFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Update(deltaTime);
		Draw();
		ImGuiNewFrame();
		DrawGui(deltaTime);
		ImGuiEndFrame();
		prevFrame = thisFrame;
		// Present our image to windows
		glfwSwapBuffers(window);
	}
	UnloadContent();
	//ShutdownImGui();
	Shutdown();

}

void Game::Initialize()
{
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}

	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
	// Create a new GLFW window
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, nullptr, nullptr);
	// We want GL commands to be executed for our window, so we make our window's context the current one
	glfwMakeContextCurrent(window);
	// Let glad know what function loader we are using (will call gl commands via glfw)
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	OnePlayerScn = new OnePlayer();
	TwoPlayerScn = new TwoPlayer();

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1) && 
		glfwJoystickPresent(GLFW_JOYSTICK_2) && glfwJoystickIsGamepad(GLFW_JOYSTICK_2)) {

		curScene = TwoPlayerScn;
	}
	else {
		curScene = OnePlayerScn;
	}
}

void Game::Shutdown()
{
	glfwTerminate();
}

void Game::LoadContent()
{
	//auto& ecs = GetRegistry("Test");
	//
	//entt::entity e1 = ecs.create();
	//Object& m1 = ecs.assign<Object>(e1);
	//ecs.assign<TempTransform>(e1).Scale = glm::vec3(1.0f);
	//
	//auto rotate = [](entt::entity e, float dt) {
	//	CurrentRegistry().get<TempTransform>(e).EulerRotation += glm::vec3(0, 0, 90 * dt);
	//};
	//auto& up = ecs.get_or_assign<UpdateBehaviour>(e1);
	//up.Function = rotate;
}

void Game::UnloadContent()
{
}

void Game::InitImGui()
{
	// Creates a new ImGUI context
	ImGui::CreateContext();
	// Gets our ImGUI input/output
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;
	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	// Dark mode FTW
	ImGui::StyleColorsDark();
	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}

}

void Game::ShutdownImGui()
{
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

void Game::ImGuiNewFrame()
{
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
}

void Game::ImGuiEndFrame()
{
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(window, &width, &height);
	io.DisplaySize = ImVec2(width, height);
	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(window);
	}

}

void Game::InputHandle(float dt)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !w_pressed) {
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = false;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = true;
		}
		w_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
		w_pressed = false;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !f2_pressed) {
		if (curScene == OnePlayerScn)
			curScene = TwoPlayerScn;
		else
			curScene = OnePlayerScn;

		f2_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
		f2_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !space_pressed) {
		curScene->GetCams()[0]->TogglePerspective();
		space_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		space_pressed = false;
	}

	curScene->InputHandle(window, mousePos, dt);
}

void Game::Update(float dt)
{

	InputHandle(dt);

	curScene->Update(dt);

	//Square->Update(dt);

}

void Game::Draw()
{
	curScene->Draw();
}

void Game::DrawGui(float deltaTime)
{
}
