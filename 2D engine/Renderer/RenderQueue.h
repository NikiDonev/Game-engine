#pragma once
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "BatchRenderer.h"
#include "Texture.h"
#include "../Debug/Timer.h"

struct RenderStats {
	int commandsSubmitted{ 0 };
	int commandsCulled{ 0 };
	int drawCalls{ 0 };
	int vertices{ 0 }, indices{ 0 };
	int shaderFlushes{0}, packetFlushes{0}, layoutFlushes{0}, overflowFlushes{0}, textureFlushes{0};
	void Reset() { *this = RenderStats(); }
};

struct RenderCommand {
	uint64_t sortKey{};
	
	Ref<Texture> texture;
	Ref<Shader> shader;
	const UniformPacket* packet;

	float zIndex{};
	bool transparent = false;

	const void* vertexData = nullptr;
	uint32_t vertexCount{};
	uint32_t vertexSize{};
	VertexLayout layout{};

	const uint32_t* indexData = nullptr;
	uint32_t indexCount{};

	glm::mat4 modelMatrix{ 1.0f };
	AABB worldBounds;
};

struct RenderState {
	Ref<Shader> shader;
	VertexLayout layout{};
	const UniformPacket* packet{};
};


class RenderQueue {
public:

	void Init();
	void PushCommand(RenderCommand& cmd);
	void Execute(View& view, Timer& timer);

	RenderStats stats;

private:
	uint64_t GenerateKey(const RenderCommand& cmd);

	BatchRenderer m_Renderer;
	std::vector<RenderCommand> m_Commands;
	RenderState m_State{};

	std::vector<uint8_t> m_ScratchBuffer;
};