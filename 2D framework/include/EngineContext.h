#pragma once

#include "Window.h"
#include "State.h"
#include "Input.h"
#include "SpriteRenderer.h"
#include "ShapeRenderer.h"
#include "TextureManager.h"
#include "ResourceManager.h"


struct EngineContext {
public:
	Window window;
	View mainView;
	Input input;

	StateManager stateManager;
	ResourceManager resourceManager;
	TextureManager textureManager;

	SpriteRenderer spriteRenderer;
	ShapeRenderer shapeRenderer;

	float time = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	uint32_t frameCount = 0;

	const int& width = input.m_Width;
	const int& height = input.m_Height;

	void Initialize(int width, int height, const char* title) {
		input.m_Width = width;
		input.m_Height = height = height;
		window.Init(width, height, title);
		mainView.setSize((float)width, (float)height);


		input.SetupCallbacks(window.glfwWindow);

		spriteRenderer.Init();
		shapeRenderer.Init();
	}

	void BeginFrame() {
		window.Clear();
		input.Update();


		float currentFrame = glfwGetTime();
		time = currentFrame;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EndFrame() {
		spriteRenderer.setView(mainView);
		shapeRenderer.setView(mainView);
		spriteRenderer.Draw();
		shapeRenderer.Draw();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainView.setSize(width, height);
		window.Display();
	}

	void Draw(const Shape& shape) {
		shapeRenderer.Add(shape);
	}
	void Draw(const Sprite& sprite) {
		spriteRenderer.Add(sprite);
	}

	float getAverageFPS() {
		return (float)frameCount / time;
	}
};