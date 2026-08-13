#pragma once

#include "GenericBatchRenderer.h"
#include "Sprite.h"


class SpriteRenderer {
public:
	VertexLayout spriteLayout;
	uint32_t emptyTexture{};
	uint32_t currTexIndex{};

	Shader defaultShader;
	View view;
	std::array<uint32_t, 16> textureIDs{ 0 };
	GenericBatchRenderer renderer;
	SpriteRenderer() {
	}
	void Init() {
		defaultShader.create(SHADERS "sprite.vert", SHADERS "sprite.frag");

		spriteLayout.size = sizeof(SpriteVertex);
		spriteLayout.attributes = {
			{ 2, GL_FLOAT, false, offsetof(SpriteVertex, position) },  // location 0
			{ 4, GL_FLOAT, false, offsetof(SpriteVertex, color) },     // location 1
			{ 2, GL_FLOAT, false, offsetof(SpriteVertex, texCoords) }, // location 2
			{ 1, GL_FLOAT, false, offsetof(SpriteVertex, texIndex) }   // location 3
		};
		renderer.Init(spriteLayout);

		defaultShader.use();
		for (int i = 0; i < 16; ++i) {
			std::string textureUniform = "textures[" + std::to_string(i) + "]";
			defaultShader.setInt(textureUniform, i);
		}

		//emptyTexture = TextureManager::Get().createTexture(1, 1, GL_RGB);
	}
	glm::vec2 transformPosition(const Transformable& transformable, glm::vec2 position) {
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
	void Add(const Sprite& sprite) {
		//if (!IsOnScreen(sprite)) return;
		SpriteVertex quadVertices[4];
		uint32_t spriteTexture = sprite.textureID;
		if (spriteTexture == 0) spriteTexture = emptyTexture;

		auto it = std::find(textureIDs.begin(), textureIDs.begin() + currTexIndex, spriteTexture);
		uint32_t slot;

		if (it == textureIDs.begin() + currTexIndex) {
			slot = currTexIndex;
			textureIDs[currTexIndex] = spriteTexture;
			currTexIndex++;
		}
		else {
			slot = std::distance(textureIDs.begin(), it);
		}

		quadVertices[0] = { transformPosition(sprite, {-0.5f, -0.5f}), sprite.color, {0.0f, 0.0f}, (float)slot };
		quadVertices[1] = { transformPosition(sprite, { 0.5f, -0.5f}), sprite.color, {1.0f, 0.0f}, (float)slot };
		quadVertices[2] = { transformPosition(sprite, {-0.5f,  0.5f}), sprite.color, {0.0f, 1.0f}, (float)slot };
		quadVertices[3] = { transformPosition(sprite, { 0.5f,  0.5f}), sprite.color, {1.0f, 1.0f}, (float)slot };


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

		glm::mat4 viewProj = view.getViewProjMatrix();
		defaultShader.use();
		defaultShader.setMat4("viewProj", viewProj);

		for (int i = 0; i < currTexIndex; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		}
		renderer.Flush();

		std::fill(textureIDs.begin(), textureIDs.end(), 0);
		currTexIndex = 0;
	}


private:

};