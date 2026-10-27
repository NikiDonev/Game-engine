#include "Sprite.h"


Sprite::Sprite(glm::vec2 size, const Ref<Texture>& texture, glm::vec4 color)
	: m_Size(size), texture(texture), m_Color(color) {
	LOG_INFO("Sprite constructor");
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

Sprite& Sprite::setTexCoords(const glm::vec4& TexCoords) { 
	vertices[0].texCoords = { TexCoords[0], TexCoords[1] };
	vertices[1].texCoords = { TexCoords[2], TexCoords[1] };
	vertices[2].texCoords = { TexCoords[0], TexCoords[3] };
	vertices[3].texCoords = { TexCoords[2], TexCoords[3] };
	return *this; 
};

AABB Sprite::getLocalBounds() {
	if (vertices.empty()) return AABB{};

	if (m_UpdateLocalBounds) {
		m_LocalBounds = AABB{ vertices[0].position, vertices[0].position };
		for (const auto& vertex : vertices) {
			m_LocalBounds.min.x = std::min(m_LocalBounds.min.x, vertex.position.x);
			m_LocalBounds.min.y = std::min(m_LocalBounds.min.y, vertex.position.y);
			m_LocalBounds.max.x = std::max(m_LocalBounds.max.x, vertex.position.x);
			m_LocalBounds.max.y = std::max(m_LocalBounds.max.y, vertex.position.y);
		}
		m_UpdateLocalBounds = false;
	}
	return m_LocalBounds;
}

AABB Sprite::getWorldBounds() {
	if (m_UpdateWorldBounds) {
		AABB localBounds = getLocalBounds();
		glm::vec2 p1, p2, p3, p4;
		p1 = transformPosition(localBounds.min);
		p2 = transformPosition(localBounds.max);
		p3 = transformPosition({ localBounds.min.x, localBounds.max.y });
		p4 = transformPosition({ localBounds.max.x, localBounds.min.y });

		m_WorldBounds.min.x = std::min({ p1.x, p2.x, p3.x, p4.x });
		m_WorldBounds.min.y = std::min({ p1.y, p2.y, p3.y, p4.y });
		m_WorldBounds.max.x = std::max({ p1.x, p2.x, p3.x, p4.x });
		m_WorldBounds.max.y = std::max({ p1.y, p2.y, p3.y, p4.y });
		m_UpdateWorldBounds = false;
	}
	return m_WorldBounds;
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