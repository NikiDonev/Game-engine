#pragma once
#include "Application.h"
#include "shader.h"
#include "rect.h"
#include "TextureManager.h"
#include "Renderer2D.h"

class Game : public Application {
public:
	bool mPressed = false;
	bool showDebug = true;
	Rect rect;
	Shape shape1;
	Sprite s1, s2;
	Renderer2D renderer;
	uint32_t mossTexture, deepslateTexture;

	Game(uint32_t width, uint32_t height) : Application(width, height) {
		std::cout << "Game layers successfully assigned window dimension constraints.\n";
	}

	void OnInit() override {
		renderer.Init();
		shape1.init();
		renderer.view = glm::scale(renderer.view, glm::vec3(100.0f));

		mossTexture = TextureManager::Get().loadTexture(RESOURCES_PATH "moss.png");
		deepslateTexture = TextureManager::Get().loadTexture(RESOURCES_PATH "deepslate.png");
		s1.shape = shape1;
		s1.texture = mossTexture;
		s2.shape = shape1;
		s2.texture = deepslateTexture;
	}

	void OnUpdate(float deltaTime) override {
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.Add(s1);

		glm::mat4 model = s2.model;
		s2.model = glm::translate(model, glm::vec3(0.001f, 0.0003f, 0.0f));
		renderer.Add(s2);
		renderer.DrawObjects(getWidth(), getHeight());
	}

	void OnRender() override {

	}

	void ImguiDebug() override {

	}
private:

	void processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		if (mPressed && glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
			mPressed = false;
			showDebug = !showDebug;
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			mPressed = true;
		}
	}
};