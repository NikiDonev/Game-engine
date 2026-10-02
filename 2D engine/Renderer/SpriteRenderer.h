#pragma once
#include "RenderQueue.h"
#include "Shape.h"
#include "Sprite.h"
class SpriteRenderer {
public:
	VertexLayout spriteLayout;
	Ref<Shader> spriteShader;
	void Init(RenderQueue* renderQueue, ResourceManager& resourceManager) {
		m_RenderQueue = renderQueue;
		spriteLayout.size = sizeof(SpriteVertex);
		spriteLayout.attributes = {
			{2, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, position)},
			{4, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, color)},
			{2, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, texCoords)},
			{1, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, texIndex)}
		};
		spriteShader = resourceManager.Load<Shader>(SHADERS "sprite.vert", SHADERS "sprite.frag");
	}

	void Draw(Sprite& sprite, const Ref<Shader>& shader, const UniformPacket* packet) {

		RenderCommand cmd;
		cmd.sortKey = 0;
		cmd.transparent = false;
		cmd.texture = sprite.texture;
		int32_t texSlotOffset = offsetof(SpriteVertex, texIndex);
		cmd.layout = spriteLayout;
		cmd.shader = shader;
		cmd.packet = packet;
		cmd.indexData = sprite.indices.data();
		cmd.indexCount = sprite.indices.size();
		cmd.vertexData = sprite.vertices.data();
		cmd.vertexCount = sprite.vertices.size();
		cmd.modelMatrix = sprite.getTransformMatrix();
		cmd.worldBounds = sprite.getWorldBounds();
		cmd.vertexSize = spriteLayout.size;
		cmd.zIndex = sprite.zIndex;

		m_RenderQueue->PushCommand(cmd);
	}

private:

	RenderQueue* m_RenderQueue;
};