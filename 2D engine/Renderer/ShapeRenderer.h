#pragma once
#include "RenderQueue.h"
#include "Shape.h"

class ShapeRenderer {
public:
	VertexLayout shapeLayout;

	const char* shapeShaderVertexCode =
		   "#version 330 core \n  \
			layout(location = 0) in vec2 aPos;\
			layout(location = 1) in vec4 aColor;\
			out vec4 Color;\
			uniform mat4 viewProj;\
			void main() {\
				gl_Position = viewProj * vec4(aPos, 0.0, 1.0);\
				Color = aColor;\
			}";

	const char* shapeShaderFragmentCode =
		   "#version 330 core \n \
			out vec4 FragColor;\
			in vec4 Color;\
			void main() {\
				FragColor = Color;\
			}";

	Ref<Shader> shapeShader;

	void Init(RenderQueue* renderQueue, ResourceManager& resourceManager) {
		m_RenderQueue = renderQueue;
		shapeLayout.size = sizeof(ShapeVertex);
		shapeLayout.attributes = {
			{2, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, position)},
			{4, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, color)}
		};
		shapeShader = resourceManager.Load<Shader>(shapeShaderVertexCode, shapeShaderFragmentCode);
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
		cmd.zIndex = shape.zIndex;

		m_RenderQueue->PushCommand(cmd);
	}

private:

	RenderQueue* m_RenderQueue;
};