#include "Sprite.h"


Sprite::Sprite(glm::vec2 size, const Ref<Texture>& texture, glm::vec4 color)
	: m_Size(size), texture(texture), m_Color(color) {
	generateGeometry();
}

Sprite& Sprite::setSize(const glm::vec2& size) {
	m_Size = size;
	generateGeometry();
	return *this;
}

Sprite& Sprite::setColor(const glm::vec4& color) {
	m_Color = color;
	generateGeometry();
	return *this;
}

void Sprite::generateGeometry() {
	vertices.clear();
	indices.clear();

	float hw = m_Size.x / 2.0f;
	float hh = m_Size.y / 2.0f;

	vertices.push_back({ {-hw, -hh}, m_Color, {texCoords[0], texCoords[1]}, 0.0f });
	vertices.push_back({ { hw, -hh}, m_Color, {texCoords[2], texCoords[1]}, 0.0f });
	vertices.push_back({ {-hw,  hh}, m_Color, {texCoords[0], texCoords[3]}, 0.0f });
	vertices.push_back({ { hw,  hh}, m_Color, {texCoords[2], texCoords[3]}, 0.0f });
	indices = { 0, 1, 2, 1, 3, 2 };

	markGeometryDirty();
}