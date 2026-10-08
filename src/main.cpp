#include "Core/EngineContext.h"
#include "Debug/Tests.h"

float prevFactor = 1.0f;
void zoomView(EngineContext& engine) {
	PROFILE_FUNCTION();
	if (engine.input.KeyHeld(GLFW_KEY_W)) engine.mainView.move({ 0.0f,  500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_S)) engine.mainView.move({ 0.0f, -500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_A)) engine.mainView.move({ -500 * engine.deltaTime, 0.0f });
	if (engine.input.KeyHeld(GLFW_KEY_D)) engine.mainView.move({ 500 * engine.deltaTime, 0.0f });
	float factor = engine.input.getScroll().y;

	float zoom = factor - prevFactor;
	if (zoom > 0.01f) engine.mainView.zoom(1.1f);
	else if (zoom < -0.01f) engine.mainView.zoom(0.9f);
	prevFactor = factor;
}


int main() {
	PROFILE_SESSION("Startup.json");
	EngineContext engine;
	engine.Initialize(800, 600, "2D game engine");
	engine.window.maximizeWindow();
	RegisterTests();
	int active = 0;
	tests[active].Init(engine);


	PROFILE_SESSION("Runtime.json");
	

	while (engine.window.IsOpen()) {
		PROFILE_SCOPE("Game loop");
		engine.BeginFrame();
		zoomView(engine);
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) engine.window.Close();
		ImGui::Begin("Tests");
		for (int i = 0; i < (int)tests.size(); ++i) {
			if (ImGui::Selectable(tests[i].name.c_str(), i == active) && i != active) {
				if (tests[active].Shutdown) tests[active].Shutdown();
				active = i;
				tests[active].Init(engine);
			}
		}
		ImGui::End();
		tests[active].Update(engine);


		engine.EndFrame();
	}
	if (tests[active].Shutdown) tests[active].Shutdown();
	Instrumentor::EndSession();  
}

