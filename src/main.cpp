#include "EngineContext.h"
#include "Entity.h"

int WIDTH{ 800 }, HEIGHT{ 600 };

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}
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

glm::vec2 pixelToWorld(glm::vec2 pixelCoords, const View& view) {
	int windowWidth = WIDTH, windowHeight = HEIGHT;
	if (windowWidth == 0 || windowHeight == 0) return glm::vec2(0.0f);

	// 3. Convert raw screen pixels to Normalized Device Coordinates (NDC) range [-1, 1]
	// X goes from -1 (left) to 1 (right)
	// Y goes from 1 (top) to -1 (bottom) — Notice we flip the Y axis direction here
	float ndcX = (2.0f * pixelCoords.x / windowWidth) - 1.0f;
	float ndcY = 1.0f - (2.0f * pixelCoords.y / windowHeight);

	glm::vec4 ndcPos(ndcX, ndcY, 0.0f, 1.0f);

	// 4. Invert your View-Projection matrix to reverse the rendering chain
	glm::mat4 invViewProj = glm::inverse(view.getViewProjMatrix());

	// 5. Unproject the coordinate back into the virtual world space
	glm::vec4 worldPos = invViewProj * ndcPos;

	return glm::vec2(worldPos.x, worldPos.y);
}
glm::vec2 getMouseWorldPos(EngineContext& engine, const View& view) {
	glm::vec2 mouse = engine.input.getMousePos();

	return pixelToWorld(mouse, view);
}

int main() {
	EngineContext engine;
	engine.Initialize(WIDTH, HEIGHT, "2D game engine");

	Shader customShader;

	uint32_t mossTexture = engine.textureManager.loadTexture(RESOURCES_PATH "moss.png");
	Sprite background({ 500.0f, 500.0f }, mossTexture);
	customShader.create(SHADERS "sprite.vert", SHADERS "custom.frag");
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
		//if (engine.input.KeyHeld(GLFW_KEY_W)) s1.move({ 0.0f,  5 * engine.deltaTime });
		//if (engine.input.KeyHeld(GLFW_KEY_S)) s1.move({ 0.0f, -5 * engine.deltaTime });
		//if (engine.input.KeyHeld(GLFW_KEY_D)) s1.move({  5 * engine.deltaTime, 0.0f });
		//if (engine.input.KeyHeld(GLFW_KEY_A)) s1.move({ -5 * engine.deltaTime, 0.0f });

		glm::vec2 mousePos = getMouseWorldPos(engine, engine.mainView);

		glm::vec2 bottomLeft = pixelToWorld({ 0.0f, 0.0f }, engine.mainView);
		glm::vec2 topRight = pixelToWorld({ WIDTH, HEIGHT }, engine.mainView);
		bird.setBounds({ bottomLeft, topRight });
		worm.setBounds({ bottomLeft, topRight });

		engine.Draw(background);
		glm::vec2 chase = worm.pos - bird.pos;
		bird.Update(engine.deltaTime, worm.pos, worm.vel);
		worm.Update(engine.deltaTime, bird.pos, bird.vel);


		ImGuiBegin();
		ImGui::Text("FPS: %f, DeltaTime : %f ms", 1.0f / engine.deltaTime, engine.deltaTime * 1000.0f);
		ImGui::Text("width %i, height %i", WIDTH, HEIGHT);
		ImGui::Text("View zoom %f, input zoom %f", engine.mainView.getZoom(), engine.input.getScroll().y);
		bird.Draw(engine.shapeRenderer);
		worm.Draw(engine.shapeRenderer);
		ImGuiEnd();
		engine.mainView.setSize(WIDTH, HEIGHT);

		engine.EndFrame();
	}
}