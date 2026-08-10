#pragma once

#include "GenericBatchRenderer.h"
#include "Sprite.h"




class SpriteRenderer {
public:
	VertexLayout spriteLayout;
	uint32_t emptyTexture{};
	uint32_t currTexIndex{};
	SpriteRenderer() {
	}
	void Init() {
		shader.create(SHADERS "sprite.vert", SHADERS "sprite.frag");

		spriteLayout.size = sizeof(SpriteVertex);
		spriteLayout.attributes = {
			{ 2, GL_FLOAT, false, offsetof(SpriteVertex, position) },  // location 0
			{ 4, GL_FLOAT, false, offsetof(SpriteVertex, color) },     // location 1
			{ 2, GL_FLOAT, false, offsetof(SpriteVertex, texCoords) },  // location 2
			{ 1, GL_FLOAT, false, offsetof(SpriteVertex, texIndex) }   // location 3
		};
		renderer.Init(spriteLayout);

		shader.use();
		for (int i = 0; i < 16; ++i) {
			std::string textureUniform = "textures[" + std::to_string(i) + "]";
			shader.setInt(textureUniform, i);
		}

		emptyTexture = TextureManager::Get().createTexture(1, 1, GL_RGB);

	}
	glm::vec2 transformPosition(Transform transform, glm::vec2 position) {
		if (abs(transform.rotation) < 0.001f) {
			return glm::vec2(transform.getMatrix() * glm::vec4(position, 0.0f, 1.0f));
			return (position * transform.scale) + transform.pos;
		}
		else {
			return glm::vec2(transform.getMatrix() * glm::vec4(position, 0.0f, 1.0f));
		}
	}
	void setView(const View& newView) {
		view = newView;
	}
	void Add(const Sprite& sprite) {
		std::vector<SpriteVertex> quadVertices;
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

		quadVertices.push_back({ transformPosition(sprite.transform, {-0.5f, -0.5f}), sprite.color, {0.0f, 0.0f}, (float)slot });
		quadVertices.push_back({ transformPosition(sprite.transform, { 0.5f, -0.5f}), sprite.color, {1.0f, 0.0f}, (float)slot });
		quadVertices.push_back({ transformPosition(sprite.transform, {-0.5f,  0.5f}), sprite.color, {0.0f, 1.0f}, (float)slot });
		quadVertices.push_back({ transformPosition(sprite.transform, { 0.5f,  0.5f}), sprite.color, {1.0f, 1.0f}, (float)slot });


		uint32_t quadIndices[6] = { 0, 1, 2, 1, 3, 2 };
		renderer.PushGeometry(quadVertices.data(), 4, quadIndices, 6);
	}
	bool IsOnScreen(const Sprite& sprite) {
		glm::vec2 camPos = view.pos;
		float halfWidth = view.width / 2.0f;
		float halfHeight = view.height / 2.0f;

		float camLeft = camPos.x - halfWidth;
		float camRight = camPos.x + halfWidth;
		float camBottom = camPos.y - halfHeight;
		float camTop = camPos.y + halfHeight;

		float spriteHalfX = sprite.transform.scale.x / 2.0f;
		float spriteHalfY = sprite.transform.scale.y / 2.0f;

		float spriteLeft = sprite.transform.pos.x - spriteHalfX;
		float spriteRight = sprite.transform.pos.x + spriteHalfX;
		float spriteBottom = sprite.transform.pos.y - spriteHalfY;
		float spriteTop = sprite.transform.pos.y + spriteHalfY;

		return (spriteRight >= camLeft && spriteLeft <= camRight &&
			spriteTop >= camBottom && spriteBottom <= camTop);
	}

	void DrawSprites() {

		glm::mat4 viewProj = view.getViewProjMatrix();
		shader.use();
		shader.setMat4("viewProj", viewProj);
		for (int i = 0; i < currTexIndex; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		}
		renderer.Flush();

		std::fill(textureIDs.begin(), textureIDs.end(), 0);
		currTexIndex = 0;
	}

	Shader shader;
	View view;
	std::array<uint32_t, 16> textureIDs{ 0 };
	GenericBatchRenderer renderer;
private:

};