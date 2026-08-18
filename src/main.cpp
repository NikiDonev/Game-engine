#include "EngineContext.h"
#include "Texture.h"

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


	Ref<Texture> moss = engine.resourceManager.Load<Texture>(RESOURCES_PATH "mosse.png");
	uint8_t data[4] = { 255, 0, 0, 255 };
	Ref<Texture> custom = engine.resourceManager.Load<Texture>("runtime://custom", data, 1, 1, 4);
	Ref<Texture> white = Texture::GetWhiteTexture();
	Ref<Texture> error = Texture::GetErrorTexture();
	Sprite background({ 500.0f, 500.0f }, custom->GetID());

	engine.mainView.setZoom(0.1f);
	glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f }, blue = { 0.0f, 0.0f, 1.0f, 1.0f }, green = { 0.0f, 1.0f, 0.0f, 1.0f },
		black = { 0.0f, 0.0f, 0.0f, 1.0f }, yellow = { 1.0f, 1.0f, 0.0f, 1.0f };

	while (engine.window.IsOpen()) {
		engine.BeginFrame();

		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);

		engine.Draw(background);
		engine.Draw(Circle(10.0f, red, 100).setOutline(1.0f, black).setScale({ 2.0f, 1.0f }));
		engine.Draw(Rect({ 10.0f, 6.0f }, blue).setPosition({ 20.0f, 0.0f }).setOutline(-0.1f, yellow));
		engine.Draw(Line({ 1.0f, 4.0f }, { 10.0f, -6.0f }, 1.0f, black).setOutline(1.0f, red));

		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / engine.deltaTime, engine.deltaTime * 1000.0f);
		ImGui::Text("Average FPS: %f", engine.getAverageFPS());
		ImGui::Text("width %i, height %i", engine.width, engine.height);
		ImGui::Text("View zoom %f, input zoom %f", engine.mainView.getZoom(), engine.input.getScroll().y);
		ImGui::End();
		

		engine.EndFrame();
	}
}

