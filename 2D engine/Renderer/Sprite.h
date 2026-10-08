#pragma once
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "View.h"
#include "Texture.h"
#include "TextureAtlas.h"

struct SpriteVertex {
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float texIndex;
};


//struct Sprite : public Transformable {
//public:
//    glm::vec4 color{ 1.0f };
//    Ref<Texture> texture;
//    glm::vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
//    Sprite() {}
//    Sprite(const glm::vec2& size, Ref<Texture> textureRef = nullptr, glm::vec4 Color = glm::vec4(1.0f))
//        : texture(textureRef), color(Color) {
//        setScale(size);
//    }
//    Sprite& setTexCoords(const glm::vec4& TexCoords) { texCoords = TexCoords; return *this; };
//    glm::vec4 getTexCoords() const { return texCoords; };
//    void useAtlas(const TextureAtlas& atlas, int index) { setTexCoords(atlas.getTexCoords(index));}
//
//    Sprite& setColor(glm::vec4 Color) { color = Color; return *this; }
//    Sprite& setTexture(Ref<Texture> textureRef) { texture = textureRef; return *this; }
//
//    Sprite& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
//    Sprite& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
//    Sprite& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
//    Sprite& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }
//
//    Sprite& move(const glm::vec2& offset) { Transformable::move(offset); return *this; }
//    Sprite& rotate(float degrees) { Transformable::rotate(degrees); return *this; }
//    Sprite& scale(const glm::vec2& factor) { Transformable::scale(factor); return *this; }
//};


class Sprite : public Transformable {
public:

	float zIndex{};
	std::vector<SpriteVertex> vertices;
	std::vector<uint32_t> indices;

	Ref<Texture> texture;
	glm::vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };

	Sprite() = default;
	Sprite(glm::vec2 size, const Ref<Texture>& texture = nullptr, glm::vec4 color = glm::vec4(1.0f));

	Sprite& setTexCoords(const glm::vec4& TexCoords) { texCoords = TexCoords; return *this; };
	glm::vec4 getTexCoords() const { return texCoords; };
	void useAtlas(const TextureAtlas& atlas, int index) { setTexCoords(atlas.getTexCoords(index)); }
	Sprite& setTexture(Ref<Texture> textureRef) { texture = textureRef; return *this; }

	Sprite& setSize(const glm::vec2& size);
	Sprite& setColor(const glm::vec4& color);
	Sprite& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Sprite& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Sprite& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Sprite& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

	Sprite& move(const glm::vec2& offset) { Transformable::move(offset); return *this; }
	Sprite& rotate(float degrees) { Transformable::rotate(degrees); return *this; }
	Sprite& scale(const glm::vec2& factor) { Transformable::scale(factor); return *this; }

	AABB getLocalBounds() {
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
	glm::vec2 transformPosition(glm::vec2 position) {
		if (std::fabs(getRotation()) < 0.001f) {
			return (position - getOrigin()) * getScale() + getPosition();
		}
		else {
			return glm::vec2(getTransformMatrix() * glm::vec4(position, 0.0f, 1.0f));
		}
	}
	AABB getWorldBounds() {
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



private:
	AABB m_LocalBounds;
	AABB m_WorldBounds;
	bool m_UpdateLocalBounds{ true };
	bool m_UpdateWorldBounds{ true };
	void markDirty() override { m_NeedUpdate = true; m_UpdateWorldBounds = true; }
	void markGeometryDirty() { m_UpdateLocalBounds = true; m_UpdateWorldBounds = true; }
	void generateGeometry();

	glm::vec2 m_Size{ 1.0f, 1.0f };
	glm::vec4 m_Color{ 1.0f };
};





