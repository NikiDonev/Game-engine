#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>

#include "openglErrorReporting.h"


void ImGuiBegin();
void ImGuiEnd();



class Window {
public:
	GLFWwindow* glfwWindow = nullptr;

	Window() = default;

	~Window();


	void Init(uint32_t width, uint32_t height, const char* title);

	bool IsOpen() { return !glfwWindowShouldClose(glfwWindow); }

	void Close() { glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE); }

	void Clear() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glm::ivec2 Window::getSize() const {
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);
		return { width, height };
	}

	void maximizeWindow() {
		glfwMaximizeWindow(glfwWindow);
	}
	void setSize(int width, int height) {
		glfwSetWindowSize(glfwWindow, width, height);
	}

	int getWidth() const {
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);
		return width;
	}
	int getHeight() const {
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);
		return height;
	}

	void Display() {
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}


private:
};


