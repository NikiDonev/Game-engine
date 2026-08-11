#include "EngineContex.h"

int WIDTH{ 800 }, HEIGHT{ 600 };

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}

int main() {
	EngineContex engine;
	engine.Initialize(WIDTH, HEIGHT, "2D game engine");

	engine.mainView.scale = glm::vec2(80.0f);

	Sprite s1, s2;
	s1.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	s2.color = { 0.0f, 1.0f, 1.0f, 1.0f };

	uint32_t mossTexture = engine.textureManager.loadTexture(RESOURCES_PATH "moss.png");
	uint32_t deepslateTexture = engine.textureManager.loadTexture(RESOURCES_PATH "deepslate.png");

	s1.textureID = mossTexture;
	s2.textureID = deepslateTexture;

	while (engine.window.IsOpen()) {
		engine.BeginFrame();
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		if (engine.input.KeyHeld(GLFW_KEY_W)) s1.transform.pos.y += 5 * engine.deltaTime;
		if (engine.input.KeyHeld(GLFW_KEY_S)) s1.transform.pos.y -= 5 * engine.deltaTime;
		if (engine.input.KeyHeld(GLFW_KEY_D)) s1.transform.pos.x += 5 * engine.deltaTime;
		if (engine.input.KeyHeld(GLFW_KEY_A)) s1.transform.pos.x -= 5 * engine.deltaTime;


		s2.transform.pos.x = 3 * sin(glfwGetTime());
		s2.transform.pos.y = 3 * cos(glfwGetTime());

		engine.spriteRenderer.Add(s1);
		engine.spriteRenderer.Add(s2);


		ImGuiBegin();
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / engine.deltaTime, engine.deltaTime * 1000.0f);
		ImGui::DragFloat2("cube 1 position ", glm::value_ptr(s1.transform.pos));
		ImGui::Text("width %i, height %i", WIDTH, HEIGHT);
		ImGuiEnd();
		engine.mainView.setSize(WIDTH, HEIGHT);

		engine.EndFrame();
	}
}