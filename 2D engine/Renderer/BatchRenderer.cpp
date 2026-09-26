#include "BatchRenderer.h"
#include "../Debug/Logging.h"

void BatchRenderer::Init() {
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
	glGenVertexArrays(1, &m_VAO);


	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, maxVertexBytes * sizeof(uint8_t), 0, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(uint32_t), 0, GL_DYNAMIC_DRAW);

	m_IndexBuffer.reserve(maxIndices);
	m_VertexBuffer.reserve(maxVertexBytes);

}

bool BatchRenderer::Flush(const VertexLayout& currentLayout) {
	if (m_VertexBuffer.empty()) return false;

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (m_VertexBuffer.size() <= maxVertexBytes)
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexBuffer.size() * sizeof(uint8_t), m_VertexBuffer.data());
	else LOG_ERROR("Vertex buffer overflow \n");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	if (m_IndexBuffer.size() <= maxIndices)
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_IndexBuffer.size() * sizeof(uint32_t), m_IndexBuffer.data());
	else LOG_ERROR("Index buffer overflow \n");

	// TODO: change vertex layout only when needed
	for (int i = 0; i < currentLayout.attributes.size(); ++i) {
		const VertexAttribute& attrib = currentLayout.attributes[i];
		glEnableVertexAttribArray(i);
		GLboolean normalized = (attrib.normalized ? GL_TRUE : GL_FALSE);
		glVertexAttribPointer(i, attrib.count, attrib.type, normalized, currentLayout.size, (void*)attrib.offset);
	}

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);

	for (int i = 0; i < currentLayout.attributes.size(); ++i) {
		glDisableVertexAttribArray(i);
	}

	glBindVertexArray(0);
	m_VertexBuffer.clear();
	m_IndexBuffer.clear();
	m_CurrentVertexCount = 0;

	return true;
}

void BatchRenderer::PushGeometry(const void* vertexData, uint32_t vertexCount, uint32_t vertexByteSize, const uint32_t* indexData, uint32_t indexCount) {
	uint32_t newVerticesSize = vertexCount * vertexByteSize;
	const uint8_t* bytePointer = reinterpret_cast<const uint8_t*>(vertexData);
	m_VertexBuffer.insert(m_VertexBuffer.end(), bytePointer, bytePointer + newVerticesSize);

	for (int i = 0; i < indexCount; ++i) {
		m_IndexBuffer.push_back(indexData[i] + m_CurrentVertexCount);
	}
	m_CurrentVertexCount += vertexCount;
}

bool BatchRenderer::WillBufferOverflow(uint32_t vertexCount, uint32_t vertexSize, uint32_t indexCount) {
	if (m_IndexBuffer.size() + indexCount > maxIndices) return true;
	if (m_VertexBuffer.size() + vertexCount * vertexSize > maxVertexBytes) return true;
	return false;
}