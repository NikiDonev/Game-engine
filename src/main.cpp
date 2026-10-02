#include "Core/EngineContext.h"
#include "Debug/Logging.h"
#include "Debug/Instrumentor.h"
#include "Renderer/Sprite.h"
#include "Renderer/SpriteRenderer.h"

float prevFactor = 0.0f;
glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f }, blue = { 0.0f, 0.0f, 1.0f, 0.5f }, green = { 0.0f, 1.0f, 0.0f, 1.0f },
black = { 0.0f, 0.0f, 0.0f, 1.0f }, yellow = { 1.0f, 1.0f, 0.0f, 1.0f }, white = {1.0f, 1.0f, 1.0f, 1.0f};

bool grateVisible = true;

void moveView(EngineContext& engine) {
	PROFILE_FUNCTION();
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
	PROFILE_SESSION("Startup.json");
	EngineContext engine;
	engine.Initialize(800, 600, "2D game engine");
	engine.window.maximizeWindow();

	Ref<Shader> spriteShader = engine.resourceManager.Load<Shader>(SHADERS "sprite.vert", SHADERS "sprite.frag");

	RoundedRect rect = RoundedRect({ 40.0f, 60.0f }, 6, blue).setPosition({ 0.0f , 0.0f }).setOutline(1.0f, {0.8f, 0.1f, 1.0f, 1.0f});
	rect.setOrigin({ 20.0f, 30.0f });

	Ref<Texture> texture = engine.resourceManager.Load<Texture>(RESOURCES_PATH "copperGrate.png");
	Sprite grate({ 100.0f, 100.0f }, texture);

	
	Ref<Shader> customShader = engine.resourceManager.Load<Shader>(SHADERS "shape.vert", SHADERS "shape.frag");


	PROFILE_SESSION("Runtime.json");
	

	while (engine.window.IsOpen()) {
		PROFILE_SCOPE("Game loop");
		engine.BeginFrame();
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);
		
		if (engine.input.KeyReleased(GLFW_KEY_T)) {
			grateVisible = !grateVisible;
		}

		rect.rotate(0.05f);
		rect.zIndex = 1.0f;


		//engine.Draw(rect);
		if(grateVisible) engine.Draw(grate);
		//engine.spriteRenderer.Draw(grate, spriteShader, &engine.packet);

		engine.EndFrame();
	}
	Instrumentor::EndSession();  
}

