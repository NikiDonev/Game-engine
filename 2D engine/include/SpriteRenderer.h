#pragma once

#include "GenericBatchRenderer.h"
#include "Sprite.h"
#include "TextureAtlas.h"


class SpriteRenderer {
public:
	VertexLayout spriteLayout;
	uint32_t currTexIndex{};


	Ref<Shader> defaultShader;
	View view;
	std::array<Ref<Texture>, 16> textureRefs{};
	GenericBatchRenderer renderer;
	SpriteRenderer() {
	}
	void Init() {

		spriteLayout.size = sizeof(SpriteVertex);
		spriteLayout.attributes = {
			{ 2, GL_FLOAT, false, offsetof(SpriteVertex, position) },  // location 0
			{ 4, GL_FLOAT, false, offsetof(SpriteVertex, color) },     // location 1
			{ 2, GL_FLOAT, false, offsetof(SpriteVertex, texCoords) }, // location 2
			{ 1, GL_FLOAT, false, offsetof(SpriteVertex, texIndex) }   // location 3
		};
		renderer.Init(spriteLayout);

		defaultShader->use();
		for (int i = 0; i < 16; ++i) {
			std::string textureUniform = "textures[" + std::to_string(i) + "]";
			defaultShader->setInt(textureUniform, i);
		}
	}
	glm::vec2 transformPosition(Transformable& transformable, glm::vec2 position) {
		if (abs(transformable.getRotation()) < 0.001f) {
			return (position * transformable.getScale()) + transformable.getPosition();
		}
		else {
			return glm::vec2(transformable.getTransformMatrix() * glm::vec4(position, 0.0f, 1.0f));
		}
	}
	void setView(const View& newView) {
		view = newView;
	}
	uint32_t getTextureSlot(Ref<Texture> spriteTexture) {
		auto it = std::find(textureRefs.begin(), textureRefs.begin() + currTexIndex, spriteTexture);
		uint32_t slot;

		if (it == textureRefs.begin() + currTexIndex) {
			slot = currTexIndex;
			textureRefs[currTexIndex] = spriteTexture;
			currTexIndex++;
		}
		else {
			slot = std::distance(textureRefs.begin(), it);
		}
		return slot;
	}
	void Add(Sprite& sprite) {
		//if (!IsOnScreen(sprite)) return;
		SpriteVertex quadVertices[4];
		Ref<Texture> spriteTexture;
		if (sprite.texture) {
			spriteTexture = sprite.texture;
		}
		else {
			spriteTexture = Texture::GetWhiteTexture();
		}

		uint32_t slot = getTextureSlot(spriteTexture);
		const auto& texCoords = sprite.getTexCoords();

		quadVertices[0] = { transformPosition(sprite, {-0.5f, -0.5f}), sprite.color, { texCoords.x, texCoords.y }, (float)slot };
		quadVertices[1] = { transformPosition(sprite, { 0.5f, -0.5f}), sprite.color, { texCoords.z, texCoords.y }, (float)slot };
		quadVertices[2] = { transformPosition(sprite, {-0.5f,  0.5f}), sprite.color, { texCoords.x, texCoords.w }, (float)slot };
		quadVertices[3] = { transformPosition(sprite, { 0.5f,  0.5f}), sprite.color, { texCoords.z, texCoords.w }, (float)slot };


		uint32_t quadIndices[6] = { 0, 1, 2, 1, 3, 2 };
		renderer.PushGeometry(&quadVertices, 4, quadIndices, 6);
	}

	bool IsOnScreen(const Sprite& sprite) {
		glm::vec2 camPos = view.getPosition();
		glm::vec2 halfView = view.getSize() / 2.0f;

		float camLeft = camPos.x - halfView.x;
		float camRight = camPos.x + halfView.x;
		float camBottom = camPos.y - halfView.y;
		float camTop = camPos.y + halfView.y;

		glm::vec2 spriteHalf = sprite.getScale() / 2.0f;

		float spriteLeft = sprite.getPosition().x - spriteHalf.x;
		float spriteRight = sprite.getPosition().x + spriteHalf.x;
		float spriteBottom = sprite.getPosition().y - spriteHalf.y;
		float spriteTop = sprite.getPosition().y + spriteHalf.y;

		return (spriteRight >= camLeft && spriteLeft <= camRight &&
			spriteTop >= camBottom && spriteBottom <= camTop);
	}

	void Draw() {
		//renderer.flushCount = 0;
		glm::mat4 viewProj = view.getViewProjMatrix();
		defaultShader->use();
		defaultShader->setMat4("viewProj", viewProj);

		for (int i = 0; i < currTexIndex; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureRefs[i]->GetID());
		}
		renderer.Flush();
		for (auto& ref : textureRefs)
			ref = nullptr;
		currTexIndex = 0;
	}


private:

};