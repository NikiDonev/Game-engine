#pragma once

#include "Window.h"
#include "State.h"
#include "Input.h"
#include "ResourceManager.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/ShapeRenderer.h"
#include "../Renderer/SpriteRenderer.h"
#include "../Debug/Timer.h"
#include "../Debug/Logging.h"
#include "../Debug/Instrumentor.h"

struct EngineContext {
public:
	Window window;
	View mainView;
	Input input;

	StateManager stateManager;
	ResourceManager resourceManager;

	RenderQueue renderQueue;

	ShapeRenderer shapeRenderer;
	SpriteRenderer spriteRenderer;


	float time = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	uint32_t frameCount = 0;

	const int& width = input.m_Width;
	const int& height = input.m_Height;
	UniformPacket packet;

	Timer timer;

	float frameHistory[120] = {};
	int historyIdx = 0;
	void Initialize(int width, int height, const char* title) {
		PROFILE_FUNCTION();
		input.m_Width = width;
		input.m_Height = height;
		window.Init(width, height, title);
		mainView.setSize((float)width, (float)height);


		input.SetupCallbacks(window.glfwWindow);

		renderQueue.Init();
		shapeRenderer.Init(&renderQueue, resourceManager);
		spriteRenderer.Init(&renderQueue, resourceManager);

	}

	void BeginFrame() {
		PROFILE_FUNCTION();
		timer.TimePoint("Start");
		window.Clear();
		input.Update();
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		time = currentFrame;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;



		DebugUI::BeginFrame();
		DebugUI::ShowLog();
		
		timer.TimePoint("Imgui");

#if DEBUG_UI
		ImGui::Begin("Profiling");
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / deltaTime, deltaTime * 1000.0f);
		ImGui::Text("%s", timer.timerData.c_str());
		float sum = 0;
		for (float v : frameHistory) sum += v;
		float avg = sum / 120.0f;

		char overlay[32];
		snprintf(overlay, sizeof(overlay), "avg %.2f ms", avg);
		ImGui::PlotLines("Frame ms", frameHistory, 120, historyIdx,
			overlay, 0.0f, 33.0f, ImVec2(240, 100));


		auto& s = renderQueue.stats;
		ImGui::Text("submitted %d  culled %d  draw calls %d", s.commandsSubmitted, s.commandsCulled, s.drawCalls);
		ImGui::Text("vertices %i  indices %i", s.vertices, s.indices);
		ImGui::Text("flush reason: shader %d  layout %d  packet %d  overflow %d",
			s.shaderFlushes, s.layoutFlushes, s.packetFlushes, s.overflowFlushes);

		ImGui::End();
#endif
		timer.TimePoint("user loop");
	}

	void EndFrame() {
		PROFILE_FUNCTION();
		packet.uniforms["viewProj"] = mainView.getViewProjMatrix();
		

		renderQueue.Execute(mainView, timer);

		timer.TimePoint("swap buffers");

		DebugUI::EndFrame();


		mainView.setSize(width, height);
		window.Display();

		frameHistory[historyIdx] = deltaTime * 1000.0f; // ms
		historyIdx = (historyIdx + 1) % 120;

		timer.Reset();
	}

	void Draw(Shape& shape) {
		shapeRenderer.Draw(shape, shapeRenderer.shapeShader, &packet);
	}
	void Draw(Sprite& sprite) {
		spriteRenderer.Draw(sprite, spriteRenderer.spriteShader, &packet);
	}
};