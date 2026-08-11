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


void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

void ImGuiBegin();
void ImGuiEnd();



class Window {
public:
	GLFWwindow* glfwWindow = nullptr;

	Window() = default;

	~Window() {
		if (glfwWindow) {
			glfwDestroyWindow(glfwWindow);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}

	void Init(uint32_t width, uint32_t height, const char* title) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (glfwWindow == NULL) {
			std::cerr << "Failed to create a window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(glfwWindow);
		glfwSwapInterval(0);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize glad" << std::endl;
			return;
		}

		glViewport(0, 0, width, height);

		glfwSetFramebufferSizeCallback(glfwWindow, frame_buffer_size_callback);


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();


		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, false);
		ImGui_ImplOpenGL3_Init("#version 330");

		enableReportGlErrors();
	}

	bool IsOpen() { return !glfwWindowShouldClose(glfwWindow); }

	void Close() { glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE); }

	void Clear() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glm::ivec2 getSize() const{
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);
		return { width, height };
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


