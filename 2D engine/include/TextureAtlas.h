#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>

class TextureAtlas {
public:
	TextureAtlas::TextureAtlas(const Ref<Texture>& textureRef, uint32_t verticalCount, uint32_t horizontalCount)
		: texture(textureRef), horizontalCount(horizontalCount), verticalCount(verticalCount) {
		uint32_t height = texture->GetHeight(), width = texture->GetWidth();
		m_SpriteWidth = width / horizontalCount;
		m_SpriteHeight = height / verticalCount;
		std::cout << width << " " << m_SpriteWidth << " " << height << " " << m_SpriteHeight << std::endl;
	}

	glm::vec4 TextureAtlas::getTexCoords(glm::ivec2 selected,float correction = 0.05f) const {
		int& x = selected.x, y = verticalCount - selected.y - 1;
		float height = (float)texture->GetHeight(), width = (float)texture->GetWidth();
		float top = (y + 1) * m_SpriteHeight - correction, bottom = y * m_SpriteHeight + correction;
		float left = x * m_SpriteWidth + correction, right = (x + 1) * m_SpriteWidth - correction;
		return { left / width, bottom / height, right / width, top / height };
	}
	glm::vec4 TextureAtlas::getTexCoords(int atlasIndex, float correction = 0.05f) const {
		int x = atlasIndex % verticalCount, y = atlasIndex / verticalCount;
		return getTexCoords({ x,y }, correction);
	}
private:
	Ref<Texture> texture;
	uint32_t verticalCount, horizontalCount;
	uint32_t m_SpriteWidth{}, m_SpriteHeight{};
};