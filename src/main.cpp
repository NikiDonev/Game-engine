#include "EngineContext.h"

int WIDTH{ 800 }, HEIGHT{ 600 };

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}
float prevFactor = 0.0f;

void moveView(EngineContext& engine) {
	if (engine.input.KeyHeld(GLFW_KEY_UP)) engine.mainView.move({ 0.0f,   -500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_DOWN)) engine.mainView.move({ 0.0f,  500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_LEFT)) engine.mainView.move({ 500 * engine.deltaTime, 0.0f });
	if (engine.input.KeyHeld(GLFW_KEY_RIGHT)) engine.mainView.move({ -500 * engine.deltaTime, 0.0f });
	float factor = engine.input.getScroll().y;

	float zoom = factor - prevFactor;
	if (zoom > 0.01f) engine.mainView.zoom(1.1f);
	else if (zoom < -0.01f) engine.mainView.zoom(0.9f);
	prevFactor = factor;


	if (engine.input.KeyHeld(GLFW_KEY_Z)) engine.mainView.scale(glm::vec2{ 200 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_X)) engine.mainView.scale(glm::vec2{ 1.0f / 2.0f * engine.deltaTime });
}

int main() {

	EngineContext engine;
	engine.Initialize(WIDTH, HEIGHT, "2D game engine");

	Shader customShader;
	customShader.create(SHADERS "sprite.vert", SHADERS "custom.frag");


	glm::vec2 position{1.0f};
	glm::vec2 velocity{8.0f, 5.0f};
	Arrow vector(velocity, 0.1f, 0.2f, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	vector.setPosition(position);

	engine.mainView.setZoom(0.01);

	while (engine.window.IsOpen()) {
		engine.BeginFrame();
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);
		//if (engine.input.KeyHeld(GLFW_KEY_W)) s1.move({ 0.0f,  5 * engine.deltaTime });
		//if (engine.input.KeyHeld(GLFW_KEY_S)) s1.move({ 0.0f, -5 * engine.deltaTime });
		//if (engine.input.KeyHeld(GLFW_KEY_D)) s1.move({  5 * engine.deltaTime, 0.0f });
		//if (engine.input.KeyHeld(GLFW_KEY_A)) s1.move({ -5 * engine.deltaTime, 0.0f });






		ImGuiBegin();
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / engine.deltaTime, engine.deltaTime * 1000.0f);
		ImGui::DragFloat2("Position", glm::value_ptr(position));
		ImGui::DragFloat2("Velocity", glm::value_ptr(velocity));
		ImGui::Text("width %i, height %i", WIDTH, HEIGHT);
		ImGui::Text("View zoom %f, input zoom %f", engine.mainView.getZoom(), engine.input.getScroll().y);
		ImGuiEnd();
		engine.mainView.setSize(WIDTH, HEIGHT);

		engine.EndFrame();
	}
}