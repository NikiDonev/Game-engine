#include "Core/EngineContext.h"
#include "Debug/Testing.h"


float prevFactor = 0.0f;
glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f }, blue = { 0.0f, 0.0f, 1.0f, 0.5f }, green = { 0.0f, 1.0f, 0.0f, 1.0f },
black = { 0.0f, 0.0f, 0.0f, 1.0f }, yellow = { 1.0f, 1.0f, 0.0f, 1.0f }, white = {1.0f, 1.0f, 1.0f, 1.0f};


void moveView(EngineContext& engine) {
	PROFILE_FUNCTION();
	if (engine.input.KeyHeld(GLFW_KEY_W)) engine.mainView.move({    0.0f,  500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_S)) engine.mainView.move({  0.0f, -500 * engine.deltaTime });
	if (engine.input.KeyHeld(GLFW_KEY_A)) engine.mainView.move({  -500 * engine.deltaTime, 0.0f });
	if (engine.input.KeyHeld(GLFW_KEY_D)) engine.mainView.move({  500 * engine.deltaTime, 0.0f });
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

	//tests.push_back(Test());
	Test test;
	test.name = "Best rest test guest";
	test.Init = [](EngineContext& engine) {
		
		};
	test.Update = [](EngineContext& engine) {
		engine.Draw(Circle(5.0f));
		};

	Ref<Texture> grateTexture = engine.resourceManager.Load<Texture>(RESOURCES_PATH "copperGrate.png");
	Ref<Texture> containerTexture = engine.resourceManager.Load<Texture>(RESOURCES_PATH "container.jpg");
	Ref<Texture> glassTexture = engine.resourceManager.Load<Texture>(RESOURCES_PATH "blue_glass.png");
	Ref<Texture> deepslateTexture = engine.resourceManager.Load<Texture>(RESOURCES_PATH "deepslate.png");
	Ref<Texture> darkoakTexture = engine.resourceManager.Load<Texture>(RESOURCES_PATH "dark_oak.png");

	glm::vec2 size(100.0f);
	std::vector<Sprite> sprites;
	sprites.push_back(Sprite(size, grateTexture).setColor(red));
	sprites.push_back(Sprite(size, containerTexture));
	sprites.push_back(Sprite(size, glassTexture).setColor(yellow));
	sprites.push_back(Sprite(size, deepslateTexture));
	sprites.push_back(Sprite(size, darkoakTexture));

	for (int i = 0; i < sprites.size(); ++i) {
		sprites[i].setPosition({ i * 100.0f, 600.0f - i * 120.0f });
	}


	PROFILE_SESSION("Runtime.json");
	

	while (engine.window.IsOpen()) {
		PROFILE_SCOPE("Game loop");
		engine.BeginFrame();
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);
		
		for(auto& sprite : sprites)
			engine.Draw(sprite);



		engine.EndFrame();
	}
	Instrumentor::EndSession();  
}

