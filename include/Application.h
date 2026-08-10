#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "openglErrorReporting.h"


class Application {
public:
	uint32_t WIDTH{ 800 }, HEIGHT{ 600 };
	float deltaTime = 0.0f;
	GLFWwindow* window = nullptr;

	Application();
	Application(uint32_t width = 800, uint32_t height = 600);
	~Application();

	void Run();

	virtual void OnInit();
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender();
	virtual void ImguiDebug();


	int getWidth() { return WIDTH; }
	int getHeight() { return HEIGHT; }


private:
	bool showDebug = true;
	bool isRunning = false;
	float lastFrame = 0.0f;


};

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* InitializeOpenGL(uint32_t width, uint32_t height);
void DeinitializeOpenGL();

void ImGuiBegin();
void ImGuiEnd();