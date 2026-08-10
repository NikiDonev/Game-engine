#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "View.h"
#include "shader.h"


struct VertexAttribute {
	uint32_t count;
	uint32_t type;
	bool normalized;
	uint32_t offset;
};

struct VertexLayout {
	std::vector<VertexAttribute> attributes;
	uint32_t size;
};


class GenericBatchRenderer {
public:
	uint32_t MAX_OBJECTS = 2000;
	uint32_t maxVertices = MAX_OBJECTS * 4;
	uint32_t maxIndices = MAX_OBJECTS * 6;
	uint32_t currentIndexCount = 0;

	void Init(const VertexLayout& vertexLayout, uint32_t maxObjects = 2000, uint32_t verticesPerObject = 4, uint32_t indicesPerObject = 6) {
		MAX_OBJECTS = maxObjects;
		maxVertices = MAX_OBJECTS * verticesPerObject;
		maxIndices = MAX_OBJECTS * indicesPerObject;
		layout = vertexLayout;


		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxVertices * vertexLayout.size, NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(uint32_t), NULL, GL_DYNAMIC_DRAW);

		for (int i = 0; i < vertexLayout.attributes.size(); ++i) {
			const VertexAttribute& attribute = vertexLayout.attributes[i];
			glEnableVertexAttribArray(i);
			GLboolean normalized = (attribute.normalized ? GL_TRUE : GL_FALSE);
			glVertexAttribPointer(i, attribute.count, attribute.type, normalized, vertexLayout.size, (void*)attribute.offset);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indexBuffer.reserve(maxIndices);
		vertexBuffer.reserve(maxVertices * vertexLayout.size);

	}

	void PushGeometry(const void* vertexData, uint32_t vertexCount, const uint32_t* indexData, uint32_t indexCount){
		uint32_t currentVertexCount = vertexBuffer.size() / layout.size;
		if (currentVertexCount + vertexCount > maxVertices || indexBuffer.size() + indexCount > maxIndices) {
			Flush();
		}

		uint32_t newVerticesSize = vertexCount * layout.size;
		const uint8_t* bytePointer = reinterpret_cast<const uint8_t*>(vertexData);
		vertexBuffer.insert(vertexBuffer.end(), bytePointer, bytePointer + newVerticesSize);

		for (int i = 0; i < indexCount; ++i) {
			indexBuffer.push_back(indexData[i] + currentIndexCount);
		}
		currentIndexCount += vertexCount;
	}

	void Flush() {
		if (vertexBuffer.empty()) return;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uint8_t) * vertexBuffer.size(), vertexBuffer.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * indexBuffer.size(), indexBuffer.data());

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		vertexBuffer.clear();
		indexBuffer.clear();
		currentIndexCount = 0;

	}


private:
	uint32_t VAO{}, VBO{}, EBO{};
	VertexLayout layout{};
	std::vector<uint32_t> indexBuffer;
	std::vector<uint8_t> vertexBuffer;
};