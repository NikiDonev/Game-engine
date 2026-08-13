#pragma once

#include "Window.h"
#include "State.h"
#include "Input.h"
#include "SpriteRenderer.h"
#include "ShapeRenderer.h"
#include "TextureManager.h"


struct EngineContext {
public:
	Window window;
	View mainView;
	Input input;

	StateManager stateManager;
	TextureManager textureManager;

	SpriteRenderer spriteRenderer;
	ShapeRenderer shapeRenderer;

	float time = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	void Initialize(int width, int height, const char* title) {
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
	}

	void EndFrame() {

		spriteRenderer.setView(mainView);
		shapeRenderer.setView(mainView);
		spriteRenderer.Draw();
		shapeRenderer.Draw();
		window.Display();
	}

	void Draw(const Shape& shape) {
		shapeRenderer.Add(shape);
	}
	void Draw(const Sprite& sprite) {
		spriteRenderer.Add(sprite);
	}
};