#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Renderer/Shader.h"
#include "Renderer/View.h"
#include "Renderer/Texture.h"
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

	Sprite& setTexCoords(const glm::vec4& TexCoords);
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

	AABB getLocalBounds();
	glm::vec2 transformPosition(glm::vec2 position) {
		if (std::fabs(getRotation()) < 0.001f) 
			return (position - getOrigin()) * getScale() + getPosition();
		else 
			return glm::vec2(getTransformMatrix() * glm::vec4(position, 0.0f, 1.0f));
	}
	AABB getWorldBounds();

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





