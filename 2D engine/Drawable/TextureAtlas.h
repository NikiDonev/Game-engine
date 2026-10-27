#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Renderer/Texture.h"
#include "Debug/Logging.h"

class TextureAtlas {
public:
	TextureAtlas() {}


	void SetDimentions(const Ref<Texture>& textureRef, uint32_t X, uint32_t Y){
		xCount = X;
		yCount = Y;
		height = textureRef->GetHeight(), width = textureRef->GetWidth();
		sliceWidth = width / xCount;
		sliceHeight = height / yCount;
	}

	glm::vec4 getTexCoords(glm::ivec2 selected,float correction = 0.05f) const {
		int x = selected.x, y = yCount - selected.y - 1;
		float top = (y + 1) * sliceHeight - correction, bottom = y * sliceHeight + correction;
		float left = x * sliceWidth + correction, right = (x + 1) * sliceWidth - correction;
		return { left / width, bottom / height, right / width, top / height };
	}
	glm::vec4 getTexCoords(int atlasIndex, float correction = 0.05f) const {
		int x = atlasIndex % yCount, y = atlasIndex / yCount;
		return getTexCoords({ x,y }, correction);
	}
	float width{}, height{};
	uint32_t xCount{ 1 }, yCount{ 1 };
	float sliceWidth{}, sliceHeight{};
private:
};
