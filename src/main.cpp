#include "Core/EngineContext.h"
#include "Debug/Logging.h"


float prevFactor = 0.0f;
glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f }, blue = { 0.0f, 0.0f, 1.0f, 0.5f }, green = { 0.0f, 1.0f, 0.0f, 1.0f },
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


int main() {
	EngineContext engine;
	engine.Initialize(800, 600, "2D game engine");
	engine.window.maximizeWindow();

	RoundedRect rect = RoundedRect({ 40.0f, 60.0f }, 6, blue).setPosition({ 0.0f , 0.0f }).setOutline(1.0f, {0.8f, 0.1f, 1.0f, 1.0f});

	rect.setOrigin({ 20.0f, 30.0f });

	glm::vec2 dir = { 10.0f, 60.0f };
	Arrow vector = Arrow(dir).setColor(red);

	Circle circle = Circle(20.0f, red).setPosition({ -100.0f, -100.0f });

	std::vector<Rect> shapes;
	int shapeCount = 200;

	Ref<Shader> customShader = engine.resourceManager.Load<Shader>(SHADERS "shape.vert", SHADERS "shape.frag");

	for (int i = 0; i < shapeCount; ++i) {
		int width = sqrt(shapeCount);
		shapes.push_back(Rect({ 40.0f, 40.0f }, green).setPosition({ i / width * 100.0f, (i % width) * 100.0f }));
		shapes.back().zIndex = 10.0f;
	}

	while (engine.window.IsOpen()) {
		engine.BeginFrame();
		if (engine.input.KeyHeld(GLFW_KEY_ESCAPE)) {
			engine.window.Close();
		}
		moveView(engine);
		
		dir.x = 100 * sin(engine.time);
		dir.y = 100 * cos(engine.time);
		vector.setDirection(dir);

		rect.rotate(0.05f);
		rect.zIndex = 1.0f;


		//engine.shapeRenderer.Draw(vector, customShader, &engine.packet);
		engine.Draw(circle);
		engine.Draw(rect);


		for (int i = 0; i < shapes.size(); ++i) {
			float offset = 0.1 * sin( engine.time);
			//shapes[i].move({ offset, 0.0f });
			engine.Draw(shapes[i]);
			engine.shapeRenderer.Draw(vector.setPosition(shapes[i].getPosition()), customShader, &engine.packet);
		}
		
		engine.EndFrame();
	}
}

