#pragma once

#include "Window.h"
#include "State.h"
#include "Input.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"


struct EngineContex {
public:
	Window window;
	View mainView;
	Input input;

	StateManager stateManager;
	TextureManager textureManager;

	SpriteRenderer spriteRenderer;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	void Initialize(int width, int height, const char* title) {
		window.Init(width, height, title);
		mainView.setSize(width, height);


		input.SetupCallbacks(window.glfwWindow);

		spriteRenderer.Init();
	}

	void BeginFrame() {
		window.Clear();
		input.Update();


		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void EndFrame() {

		spriteRenderer.setView(mainView);
		spriteRenderer.DrawSprites();
		window.Display();
	}
};