#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Sprite.h"
#include "View.h"




class Renderer2D {
public:

	int MAX_OBJECTS = 2000;
	int MAX_VERTICES = MAX_OBJECTS * 4;
	int MAX_INDICES = MAX_OBJECTS * 6;
	uint32_t currIndex = 0, currTexIndex = 0;

	Renderer2D() {
	}
	void Init() {
		shader.create(SHADERS "sprite.vert", SHADERS "sprite.frag");

		vertices.reserve(MAX_VERTICES);
		indices.reserve(MAX_INDICES	);

		glGenBuffers(1, &spriteVBO);
		glGenBuffers(1, &spriteEBO);
		glGenVertexArrays(1, &spriteVAO);

		glBindVertexArray(spriteVAO);

		glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MAX_INDICES, NULL, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, color));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texCoords));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texIndex));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		shader.use();
		for (int i = 0; i < 16; ++i) {
			std::string textureUniform = "textures[" + std::to_string(i) + "]";
			shader.setInt(textureUniform, i);
		}

		emptyTexture = TextureManager::Get().createTexture(1, 1, GL_RGB);

	}
	glm::vec2 transformPosition(Transform transform, glm::vec2 position) {
		if (abs(transform.rotation) < 0.001f) {
			return (position * transform.scale) + transform.pos;
		}
		else {
			return glm::vec2(transform.getMatrix() * glm::vec4(position, 0.0f, 1.0f));
		}
	}
	void setView(const View& newView) {
		view = newView;
	}
	void Add(Sprite sprite) {
		if (currIndex >= MAX_VERTICES || currTexIndex >= 16) {
			DrawSprites();
		}
		if (!IsOnScreen(sprite)) return;

		if (sprite.textureID == 0) sprite.textureID = emptyTexture;

		auto it = std::find(textureIDs.begin(), textureIDs.begin() + currTexIndex, sprite.textureID);
		uint32_t slot;

		if (it == textureIDs.begin() + currTexIndex) {
			slot = currTexIndex;
			textureIDs[currTexIndex] = sprite.textureID;
			currTexIndex++;
		}
		else {
			slot = std::distance(textureIDs.begin(), it);
		}


		vertices.push_back({ transformPosition(sprite.transform, {-0.5f, -0.5f}), sprite.color, {0.0f, 0.0f}, (float)slot });
		vertices.push_back({ transformPosition(sprite.transform, { 0.5f, -0.5f}), sprite.color, {1.0f, 0.0f}, (float)slot });
		vertices.push_back({ transformPosition(sprite.transform, {-0.5f,  0.5f}), sprite.color, {0.0f, 1.0f}, (float)slot });
		vertices.push_back({ transformPosition(sprite.transform, { 0.5f,  0.5f}), sprite.color, {1.0f, 1.0f}, (float)slot });

		indices.push_back(currIndex);
		indices.push_back(currIndex + 1);
		indices.push_back(currIndex + 2);
		indices.push_back(currIndex + 1);
		indices.push_back(currIndex + 3);
		indices.push_back(currIndex + 2);

		currIndex += 4;
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

		if (vertices.empty()) { 
			return; 
		};

		glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SpriteVertex) * vertices.size(), vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * indices.size(), indices.data());


		glm::mat4 viewProj = view.getViewProjMatrix();
		shader.use();
		shader.setMat4("viewProj", viewProj);
		glBindVertexArray(spriteVAO);
		for (int i = 0; i < currTexIndex; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		}


		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		
		vertices.clear();
		indices.clear();
		std::fill(textureIDs.begin(), textureIDs.end(), 0);
		currTexIndex = 0;
		currIndex = 0;
	}
	
private:
	uint32_t spriteVAO{}, spriteVBO{}, spriteEBO{};
	std::array<uint32_t, 16> textureIDs{ 0 };
	std::vector<uint32_t> indices;
	std::vector<SpriteVertex> vertices;
	Shader shader;

	unsigned int emptyTexture{};
	View view;
};