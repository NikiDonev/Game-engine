#pragma once
#include "RenderQueue.h"
#include "Shape.h"
class SRenderer {
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

	void Draw(const Shape& shape, Ref<Shader> shader) {

		RenderCommand cmd;
		cmd.sortKey = 0;
		cmd.transparent = false;
		cmd.texture = nullptr;
		cmd.layout = shapeLayout;
		cmd.shader = shader;
		cmd.indexData = shape.indices.data();
		cmd.indexCount = shape.indices.size();
		cmd.vertexData = shape.vertices.data();
		cmd.vertexCount = shape.vertices.size();
		cmd.modelMatrix = shape.getTransformMatrix();
		cmd.localBounds = shape.getLocalBounds();
		cmd.vertexSize = shapeLayout.size;
		cmd.zIndex = 0.0f;

		m_RenderQueue->PushCommand(cmd);
	}

private:

	RenderQueue* m_RenderQueue;
};