#pragma once


#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GenericBatchRenderer.h"


class BatchRenderer {
public:
	uint32_t maxObjects = 10000;
	uint32_t maxVertices = 40000;
	uint32_t maxIndices = 60000;
	 
	uint32_t maxVertexBytes = 36 * maxVertices;

	void Init();
	void Flush(const VertexLayout& currentLayout);
	void PushGeometry(const void* vertexData, uint32_t vertexCount, uint32_t vertexByteSize, const uint32_t* indexData, uint32_t indexCount);
	bool WillBufferOverflow(uint32_t vertexCount, uint32_t vertexSize, uint32_t indexCount);

private:
	uint32_t m_VAO{}, m_VBO{}, m_EBO{};

	std::vector<uint8_t> m_VertexBuffer;
	std::vector<uint32_t> m_IndexBuffer;

	uint32_t m_CurrentVertexCount{};
};