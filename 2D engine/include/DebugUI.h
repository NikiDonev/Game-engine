#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


struct DebugUI {
	static void Init(GLFWwindow* glfwWindow);
	static void Shutdown();
	static void BeginFrame();
	static void EndFrame();
	static void ShowLog();
	static bool KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static bool MouseCallback(GLFWwindow* window, int button, int action, int mods);
	static bool ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	//static void ShowProfiler(float* frameHistory, int historyIdx, float deltaTime);
};