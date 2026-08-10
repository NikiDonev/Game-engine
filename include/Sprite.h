#pragma once
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SpriteVertex {
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float texIndex;
};

struct Transform {
	glm::vec2 pos{}, origin{};
	float rotation{};
	glm::vec2 scale = glm::vec2(1.0f);

	glm::mat4 getMatrix () const {
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(pos, 0.0f));
		model = glm::translate(model, glm::vec3(origin, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale, 1.0f));
		model = glm::translate(model, glm::vec3(-origin, 0.0f));
		return model;
	}
};

//class Shape1 {
//public:
//
//	unsigned int VBO, VAO, EBO;
//	float repeat = 1.0f;
//	std::vector<Vertex> vertices;
//	std::vector<uint32_t> indices;
//	void init() {
//		// position	       texture coords
//		vertices.push_back({ {-0.5f, -0.5f}, { 0.0f,      0.0f} });
//		vertices.push_back({ { 0.5f, -0.5f}, {  repeat,   0.0f} });
//		vertices.push_back({ {-0.5f,  0.5f}, {  0.0f,   repeat} });
//		vertices.push_back({ { 0.5f,  0.5f}, {  repeat, repeat} });
//		indices = {
//			0, 1, 2,
//			1, 3, 2 };
//
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &VBO);
//		glGenBuffers(1, &EBO);
//
//		glBindVertexArray(VAO);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
//		glEnableVertexAttribArray(1);
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	}
//	void draw() {
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//	}
//
//};

//class Shape {
//public:
//	Transform transform;
//	glm::vec4 color{ 1.0f };
//	std::vector<Vertex> vertices;
//	std::vector<uint32_t> indices;
//	Shape() {
//		//					  position	      texture coords
//		vertices.push_back({ {-0.5f, -0.5f}, { 0.0f, 0.0f} });
//		vertices.push_back({ { 0.5f, -0.5f}, { 1.0f, 0.0f} });
//		vertices.push_back({ {-0.5f,  0.5f}, { 0.0f, 1.0f} });
//		vertices.push_back({ { 0.5f,  0.5f}, { 1.0f, 1.0f} });
//		indices = {
//			0, 1, 2,
//			1, 3, 2 
//		};
//	}
//};

class Sprite {
public:
	glm::vec4 color{ 1.0f };
	Transform transform;
	uint32_t textureID{};
};


