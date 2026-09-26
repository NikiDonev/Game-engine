#include "RenderQueue.h"
#include "../Debug/Logging.h"

void RenderQueue::Init() {
	m_Renderer.Init();
}

void RenderQueue::PushCommand(RenderCommand& cmd) {
	cmd.sortKey = GenerateKey(cmd);
	m_Commands.emplace_back(cmd);
}

struct BitPacker {
	uint64_t result{};
	int i{};
	void PushBits(uint64_t bits, int bitCount) {
		if (i + bitCount > 64) LOG_WARN("Pushed too many bits");
		uint64_t mask = (1 << bitCount) - 1;
		bits &= mask;
		result <<= bitCount;
		result |= bits;
		i += bitCount;
	}
};

static uint32_t floatToUint(float f) {
	uint32_t u;
	std::memcpy(&u, &f, sizeof(f));
	uint32_t signedBit = 0x80000000u;
	if (u & signedBit) return ~u;
	return u | signedBit;
}

uint64_t RenderQueue::GenerateKey(const RenderCommand& cmd){
	BitPacker packer;

	uint64_t transparency = cmd.transparent;
	uint64_t layer = 0;
	uint64_t depth = floatToUint(cmd.zIndex);
	uint64_t shader = cmd.shader ? cmd.shader->GetID() : 0;
	uint64_t layout = cmd.layout.id;
	uint64_t texture = cmd.texture ? cmd.texture->GetID() : 0;
	uint64_t packet = cmd.packet ? cmd.packet->id : 0;
	packer.PushBits(transparency, 1);
	packer.PushBits(layer, 2);
	packer.PushBits(depth, 32);
	packer.PushBits(shader, 6);
	packer.PushBits(layout, 4);
	packer.PushBits(texture, 8);
	packer.PushBits(packet, 8);

	return packer.result;
}

void RenderQueue::Execute(View& view, Timer& timer) {
	stats.Reset();
	if (m_Commands.empty()) return;

	stats.commandsSubmitted = m_Commands.size();


	timer.TimePoint("sorting commands");

	std::stable_sort(m_Commands.begin(), m_Commands.end(), [](const RenderCommand& a, const RenderCommand& b) {
		return a.sortKey < b.sortKey;
		});	

	timer.TimePoint("loop through commands");
	for (int cmdI = 0; cmdI < m_Commands.size(); ++cmdI) {
		const RenderCommand& cmd = m_Commands[cmdI];
		if (cmd.shader == nullptr) {
			LOG_ERROR("Error: command %i didn't supply a shader \n", cmdI);
			continue;
		}
		if (cmd.packet == nullptr) {
			LOG_ERROR("Error: command %i didn't supply a uniform packet \n", cmdI);
			continue;
		}

		AABB worldBounds = cmd.worldBounds;
		AABB viewBounds = view.getFrustumBounds();
		bool isVisible = (worldBounds.max.x >= viewBounds.min.x && worldBounds.min.x <= viewBounds.max.x) &&
			(worldBounds.max.y >= viewBounds.min.y && worldBounds.min.y <= viewBounds.max.y);
		if (!isVisible) { stats.commandsCulled++; continue; }


		bool shaderChanged = (m_State.shader != cmd.shader);
		bool layoutChanged = (m_State.layout != cmd.layout);
		bool packetChanged = (m_State.packet != cmd.packet);


		//TODO: Add texture slots
		bool textureSlotsFull = false;
		bool bufferOverflow = m_Renderer.WillBufferOverflow(cmd.vertexCount, cmd.vertexSize, cmd.indexCount);



		bool needsFlush = shaderChanged || layoutChanged || packetChanged || textureSlotsFull || bufferOverflow;

		if (needsFlush) {
			if (m_Renderer.Flush(m_State.layout)) {
				stats.drawCalls++;
				if (shaderChanged) stats.shaderFlushes++;
				if (packetChanged) stats.packetFlushes++;
				if (layoutChanged) stats.layoutFlushes++;
				if (textureSlotsFull) stats.textureFlushes++;
				if (bufferOverflow) stats.overflowFlushes++;
			}
			
			if (shaderChanged) m_State.shader = cmd.shader;
			if (packetChanged) m_State.packet = cmd.packet;
			if (shaderChanged || packetChanged) m_State.shader->ApplyUniforms(*cmd.packet);
			if (layoutChanged) m_State.layout = cmd.layout;
		}



		uint32_t totalBytes = cmd.vertexCount * cmd.vertexSize;
		m_ScratchBuffer.resize(totalBytes);
		std::memcpy(m_ScratchBuffer.data(), cmd.vertexData, totalBytes);

		for (uint32_t i = 0; i < cmd.vertexCount; ++i) {
			uint8_t* currentVertexBytePtr = m_ScratchBuffer.data() + (i * cmd.vertexSize);
			glm::vec2* pos = reinterpret_cast<glm::vec2*>(currentVertexBytePtr);
			*pos = glm::vec2(cmd.modelMatrix * glm::vec4(*pos, 0.0f, 1.0f));
		}

		m_Renderer.PushGeometry(m_ScratchBuffer.data(), cmd.vertexCount, cmd.vertexSize, cmd.indexData, cmd.indexCount);
		stats.vertices += cmd.vertexCount; stats.indices += cmd.indexCount;
	}
	if(m_Renderer.Flush(m_State.layout)) stats.drawCalls++;



	m_Commands.clear();
	m_State = {};
}