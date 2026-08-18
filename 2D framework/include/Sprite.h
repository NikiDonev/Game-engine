#pragma once
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "View.h"
#include "Texture.h"

struct SpriteVertex {
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float texIndex;
};


struct Sprite : public Transformable {
public:
    glm::vec4 color{ 1.0f };
    Ref<Texture> texture;

    Sprite() {}
    Sprite(const glm::vec2& size, Ref<Texture> textureRef = nullptr, glm::vec4 Color = glm::vec4(1.0f))
        : texture(textureRef), color(Color) {
        setScale(size);
    }

    Sprite& setColor(glm::vec4 Color) { color = Color; return *this; }
    Sprite& setTexture(Ref<Texture> textureRef) { texture = textureRef; return *this; }

    Sprite& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
    Sprite& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
    Sprite& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
    Sprite& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

    Sprite& move(const glm::vec2& offset) { Transformable::move(offset); return *this; }
    Sprite& rotate(float degrees) { Transformable::rotate(degrees); return *this; }
    Sprite& scale(const glm::vec2& factor) { Transformable::scale(factor); return *this; }
};


