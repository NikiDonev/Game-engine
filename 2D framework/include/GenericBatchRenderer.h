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
	int flushCount = 0;
	uint32_t maxObjects = 2000;
	uint32_t maxVertices = maxObjects * 4;
	uint32_t maxIndices = maxObjects * 6;
	uint32_t currentIndexCount = 0, currentVertexCount = 0;
	uint32_t currentShaderID{};

	void Init(const VertexLayout& vertexLayout, uint32_t MaxObjects = 2000, uint32_t verticesPerObject = 4, uint32_t indicesPerObject = 6);

	void PushGeometry(const void* vertexData, uint32_t vertexCount, const uint32_t* indexData, uint32_t indexCount);

	void Flush();


private:
	uint32_t VAO{}, VBO{}, EBO{};
	VertexLayout layout{};
	std::vector<uint32_t> indexBuffer;
	std::vector<uint8_t> vertexBuffer;
};