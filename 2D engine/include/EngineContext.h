#pragma once

#include "Window.h"
#include "State.h"
#include "Input.h"
#include "SpriteRenderer.h"
#include "ShapeRenderer.h"
#include "ResourceManager.h"
#include "RenderQueue.h"
#include "sRenderer.h"

struct EngineContext {
public:
	Window window;
	View mainView;
	Input input;

	StateManager stateManager;
	ResourceManager resourceManager;

	RenderQueue renderQueue;
	SRenderer shapeRenderer;
	//SpriteRenderer spriteRenderer;
	//ShapeRenderer shapeRenderer;

	Ref<Shader> shapeShader;
	Ref<Shader> spriteShader;

	float time = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	uint32_t frameCount = 0;

	const int& width = input.m_Width;
	const int& height = input.m_Height;
	UniformPacket packet;
	void Initialize(int width, int height, const char* title) {
		input.m_Width = width;
		input.m_Height = height;
		window.Init(width, height, title);
		mainView.setSize((float)width, (float)height);


		input.SetupCallbacks(window.glfwWindow);

		renderQueue.Init();
		shapeRenderer.Init(&renderQueue);
		//spriteShader = resourceManager.Load<Shader>(SHADERS "sprite.vert", SHADERS "sprite.frag");
		const char* shapeShaderVertexCode = 
		   "#version 330 core \n  \
			layout(location = 0) in vec2 aPos;\
			layout(location = 1) in vec4 aColor;\
			out vec4 Color;\
			uniform mat4 viewProj;\
			void main() {\
				gl_Position = viewProj * vec4(aPos, 0.0, 1.0);\
				Color = aColor;\
			}";

		const char* shapeShaderFragmentCode = 
			"#version 330 core \n \
			out vec4 FragColor;\
			in vec4 Color;\
			void main() {\
				FragColor = Color;\
			}";

		shapeShader = resourceManager.Load<Shader>(shapeShaderVertexCode, shapeShaderFragmentCode);

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
		packet["viewProj"] = mainView.getViewProjMatrix();
		renderQueue.Execute(mainView);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainView.setSize(width, height);
		window.Display();
	}

	void Draw(const Shape& shape, const View& view) {
		
		shapeRenderer.Draw(shape, shapeShader, &packet);
	}



};