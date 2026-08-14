#include "EngineContext.h"
#include "Entity.h"


float prevFactor = 0.0f;

void moveView(EngineContext& engine) {
	if (engine.input.KeyHeld(GLFW_KEY_UP)) engine.mainView.move({    0.0f,  500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_DOWN)) engine.mainView.move({  0.0f, -500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_LEFT)) engine.mainView.move({  -500 * engine.deltaTime, 0.0f });
	if (engine.input.KeyHeld(GLFW_KEY_RIGHT)) engine.mainView.move({  500 * engine.deltaTime, 0.0f });
	float factor = engine.input.getScroll().y;

	float zoom = factor - prevFactor;
	if (zoom > 0.01f) engine.mainView.zoom(1.1f);
	else if (zoom < -0.01f) engine.mainView.zoom(0.9f);
	prevFactor = factor;
}


int main() {
	EngineContext engine;
	engine.Initialize(800, 600, "2D game engine");

	uint32_t mossTexture = engine.textureManager.loadTexture(RESOURCES_PATH "moss.png");
	Sprite background({ 500.0f, 500.0f }, mossTexture);

	Entity bird(0);
	Entity worm(1);

	worm.pos = { 10.0f, 0.0f };

	worm.direction.setColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	engine.mainView.setZoom(0.1f);


	while (engine.window.IsOpen()) {
		engine.BeginFrame();

		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);

		glm::vec2 mousePos = engine.input.getMouseWorldPos(engine.mainView);

		glm::vec2 bottomLeft = engine.input.pixelToWorld({ 0.0f, 0.0f }, engine.mainView);
		glm::vec2 topRight = engine.input.pixelToWorld({ engine.width, engine.height }, engine.mainView);
		bird.setBounds({ bottomLeft, topRight });
		worm.setBounds({ bottomLeft, topRight });

		engine.Draw(background);
		glm::vec2 chase = worm.pos - bird.pos;
		bird.Update(engine.deltaTime, worm.pos, worm.vel);
		worm.Update(engine.deltaTime, bird.pos, bird.vel);


		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / engine.deltaTime, engine.deltaTime * 1000.0f);
		ImGui::Text("width %i, height %i", engine.input.Width, engine.input.Height);
		ImGui::Text("View zoom %f, input zoom %f", engine.mainView.getZoom(), engine.input.getScroll().y);
		bird.Draw(engine.shapeRenderer);
		worm.Draw(engine.shapeRenderer);
		ImGui::End();



		engine.EndFrame();
	}
}