#include "Tests.h"
#include "Core/EngineContext.h"







namespace { std::vector<Rect> rects(2025); }

void RectTestInit(EngineContext& engine) {
	int i = 0;
	for (Rect& rect : rects) {
		rect = Rect({ 100.0f, 100.0f });
		int sqrt = std::sqrt(rects.size());
		float width = i % sqrt, height = i / sqrt;
		rect.setPosition({ width * 150.0f, height * 150.0f });
		float count = rects.size();
		float red = width / count * sqrt;
		float blue = height / count * sqrt;
		float green = (count - i) / count / 2;
		rect.setColor({ red, green, blue, 1.0f });
		rect.zIndex = 2000 - i;
		++i;
	}
}
void RectTestUpdate(EngineContext& engine) {
	for (Rect& rect : rects) {
		engine.Draw(rect);
	}
}


namespace { 
	std::vector<Sprite>sprites;
	std::vector<Ref<Texture>> textures;
	int spriteCount = 1000;
}

void TestInit(EngineContext& engine) {
	textures.clear();
	textures.push_back(engine.resourceManager.Load<Texture>(RESOURCES_PATH "copperGrate.png"));
	textures.push_back(engine.resourceManager.Load<Texture>(RESOURCES_PATH "blue_glass.png"));
	textures.push_back(engine.resourceManager.Load<Texture>(RESOURCES_PATH "deepslate.png"));
	textures.push_back(engine.resourceManager.Load<Texture>(RESOURCES_PATH "dark_oak.png"));
	textures.push_back(engine.resourceManager.Load<Texture>(RESOURCES_PATH "container.jpg"));
	sprites.resize(spriteCount);
	for (int i = 0; i < sprites.size(); ++i) {
		
		sprites[i] = Sprite({100.0f, 100.0f}, textures[i % textures.size()]);
		sprites[i].setPosition({ (i % 50) * 120.0f, (i / 50) * 120 });
	}
}
void TestUpdate(EngineContext& engine) {
	for (Sprite& sprite : sprites) {
		sprite.rotate(0.1f);
		sprite.setScale(glm::vec2(abs(sin(engine.time))));
		engine.Draw(sprite);
	}
}

void RegisterTests() {
	tests.push_back({ "2025 Rects" , RectTestInit, RectTestUpdate });
	tests.push_back({ "Texture test" , TestInit, TestUpdate });
}