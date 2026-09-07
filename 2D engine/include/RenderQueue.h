#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "BatchRenderer.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <imgui.h>

struct RenderCommand {
	uint64_t sortKey{};
	
	Ref<Texture> texture;
	Ref<Shader> shader;
	float zIndex{};
	bool transparent = false;

	const void* vertexData = nullptr;
	uint32_t vertexCount{};
	uint32_t vertexSize{};
	VertexLayout layout{};

	const uint32_t* indexData = nullptr;
	uint32_t indexCount{};

	glm::mat4 modelMatrix{ 1.0f };
	AABB localBounds;
};

struct RenderState {
	Ref<Shader> shader;
	VertexLayout layout{};
};


class RenderQueue {
public:
	void Init();
	void PushCommand(RenderCommand cmd);
	void Execute(const View& view);

private:
	uint64_t GenerateKey(const RenderCommand& cmd);

	BatchRenderer m_Renderer;
	std::vector<RenderCommand> m_Commands;
	RenderState m_State{};

	std::vector<uint8_t> m_ScratchBuffer;
};