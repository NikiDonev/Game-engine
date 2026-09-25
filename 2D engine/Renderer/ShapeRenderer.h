#pragma once
#include "RenderQueue.h"
#include "Shape.h"
class ShapeRenderer {
public:
	VertexLayout shapeLayout;

	void Init(RenderQueue* renderQueue) {
		m_RenderQueue = renderQueue;
		shapeLayout.size = sizeof(ShapeVertex);
		shapeLayout.attributes = {
			{2, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, position)},
			{4, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, color)}
		};
	}

	void Draw(Shape& shape, const Ref<Shader>& shader, const UniformPacket* packet) {

		RenderCommand cmd;
		cmd.sortKey = 0;
		cmd.transparent = false;
		cmd.texture = nullptr;
		cmd.layout = shapeLayout;
		cmd.shader = shader;
		cmd.packet = packet;
		cmd.indexData = shape.indices.data();
		cmd.indexCount = shape.indices.size();
		cmd.vertexData = shape.vertices.data();
		cmd.vertexCount = shape.vertices.size();
		cmd.modelMatrix = shape.getTransformMatrix();
		cmd.worldBounds = shape.getWorldBounds();
		cmd.vertexSize = shapeLayout.size;
		cmd.zIndex = 0.0f;

		m_RenderQueue->PushCommand(cmd);
	}

private:

	RenderQueue* m_RenderQueue;
};