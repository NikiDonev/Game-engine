#include "EngineContext.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Tilemap.h"

float prevFactor = 0.0f;
glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f }, blue = { 0.0f, 0.0f, 1.0f, 1.0f }, green = { 0.0f, 1.0f, 0.0f, 1.0f },
black = { 0.0f, 0.0f, 0.0f, 1.0f }, yellow = { 1.0f, 1.0f, 0.0f, 1.0f };

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
void setUniform(UniformPacket& packet) {
	glm::mat4 viewProj = glm::mat4(1.0f);
	float time = glfwGetTime();
	packet.Add("u_Time", time);
	int index = 4;
	packet.Add("u_Index", index);
	packet.Add("u_ViewProj", viewProj);
}

int main() {
	EngineContext engine;
	engine.Initialize(800, 600, "2D game engine");


	UniformPacket packet;
	setUniform(packet);






	engine.mainView.setZoom(0.1f);
	Rect rect = Rect({ 40.0f, 60.0f }, blue).setPosition({ 0.0f , 0.0f });
	std::vector<Rect> shapes;
	int shapeCount = 2000;

	for (int i = 0; i < shapeCount; ++i) {
		int width = sqrt(shapeCount);
		shapes.push_back(Rect({ 40.0f, 40.0f }, green).setPosition({ i / width * 100.0f, (i % width) * 100.0f }));
	}

	while (engine.window.IsOpen()) {
		engine.BeginFrame();
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);

		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / engine.deltaTime, engine.deltaTime * 1000.0f);
		ImGui::End();

		for (int i = 0; i < shapes.size(); ++i) {
			engine.Draw(shapes[i]);
		}
		
		engine.EndFrame();
	}
}

