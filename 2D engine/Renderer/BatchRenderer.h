#pragma once


#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "View.h"
#include "Shader.h"


struct VertexAttribute {
	uint32_t count{};
	uint32_t type{};
	bool normalized{};
	uint32_t offset{};
	bool operator==(const VertexAttribute& other) const{
		return ((count == other.count) && (type == other.type) && (normalized == other.normalized) && (offset == other.offset));
	}
};
struct VertexLayout {
	std::vector<VertexAttribute> attributes;
	uint32_t size{};
	uint32_t id{ 0 };
	VertexLayout() : id(s_NextID) { s_NextID++; }
	bool operator==(const VertexLayout& other) const{
		if (size != other.size) return false;
		if (attributes != other.attributes) return false;
		return true;
	}
	bool operator!=(const VertexLayout& other) const {
		return !(*this == other);
	}
private:
	static inline uint32_t s_NextID = 0;
};



class BatchRenderer {
public:
	uint32_t maxObjects = 10000;
	uint32_t maxVertices = 40000;
	uint32_t maxIndices = 60000;
	 
	uint32_t maxVertexBytes = 36 * maxVertices;

	void Init();
	bool Flush(const VertexLayout& currentLayout);
	void PushGeometry(const void* vertexData, uint32_t vertexCount, uint32_t vertexByteSize, const uint32_t* indexData, uint32_t indexCount);
	bool WillBufferOverflow(uint32_t vertexCount, uint32_t vertexSize, uint32_t indexCount);

private:
	uint32_t m_VAO{}, m_VBO{}, m_EBO{};

	std::vector<uint8_t> m_VertexBuffer;
	std::vector<uint32_t> m_IndexBuffer;

	uint32_t m_CurrentVertexCount{};
};