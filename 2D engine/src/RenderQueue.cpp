#include "RenderQueue.h"

void RenderQueue::Init() {
	m_Renderer.Init();
}

void RenderQueue::PushCommand(RenderCommand& cmd) {
	cmd.sortKey = GenerateKey(cmd);
	m_Commands.emplace_back(cmd);
}

uint64_t RenderQueue::GenerateKey(const RenderCommand& cmd){
	return 0;
}

void RenderQueue::Execute(const View& view, Timer& timer) {
	if (m_Commands.empty()) return;

	timer.TimePoint("sorting commands");

	std::sort(m_Commands.begin(), m_Commands.end(), [](const RenderCommand& a, const RenderCommand& b) {
		return a.sortKey < b.sortKey;
		});	
	int flushCounter{};

	timer.TimePoint("loop through commands");
	for (int cmdI = 0; cmdI < m_Commands.size(); ++cmdI) {
		const RenderCommand& cmd = m_Commands[cmdI];
		if (cmd.shader == nullptr) {
			std::cerr << "Error: command " << cmdI << " didn't supply a shader \n";
			continue;
		}


		bool shaderChanged = (m_State.shader != cmd.shader);
		bool layoutChanged = (m_State.layout != cmd.layout);
		bool packetChanged = (m_State.packet != cmd.packet);
		//if (m_State.packet == cmd.packet) packetChanged = (*m_State.packet == *cmd.packet);
		//TODO: Add texture slots
		bool textureSlotsFull = false;
		bool bufferOverflow = m_Renderer.WillBufferOverflow(cmd.vertexCount, cmd.vertexSize, cmd.indexCount);

		if (shaderChanged || layoutChanged || packetChanged || textureSlotsFull || bufferOverflow) {
			flushCounter++;
			m_Renderer.Flush(m_State.layout);
			
			if (shaderChanged) {
				m_State.shader = cmd.shader;
				m_State.shader->ApplyUniforms(*cmd.packet);
			}
			if (layoutChanged) {
				m_State.layout = cmd.layout;
			}
			if (packetChanged) {
				m_State.packet = cmd.packet;
			}
		}

		//AABB worldBounds;
		//worldBounds.min = glm::vec2(cmd.modelMatrix * glm::vec4(cmd.localBounds.min, 0.0f, 1.0f));
		//worldBounds.max = glm::vec2(cmd.modelMatrix * glm::vec4(cmd.localBounds.max, 0.0f, 1.0f));
		//AABB viewBounds = view.getFrustumBounds();
		//bool isVisible = (worldBounds.max.x >= viewBounds.min.x && worldBounds.min.x <= viewBounds.max.x) &&
		//	(worldBounds.max.y >= viewBounds.min.y && worldBounds.min.y <= viewBounds.max.y);
		//if (!isVisible) continue;

		uint32_t totalBytes = cmd.vertexCount * cmd.vertexSize;
		m_ScratchBuffer.resize(totalBytes);
		std::memcpy(m_ScratchBuffer.data(), cmd.vertexData, totalBytes);

		const uint8_t* rawBytes = static_cast<const uint8_t*>(cmd.vertexData);

		for (uint32_t i = 0; i < cmd.vertexCount; ++i) {
			uint8_t* currentVertexBytePtr = m_ScratchBuffer.data() + (i * cmd.vertexSize);
			glm::vec2* pos = reinterpret_cast<glm::vec2*>(currentVertexBytePtr);
			*pos = glm::vec2(cmd.modelMatrix * glm::vec4(*pos, 0.0f, 1.0f));
		}

		m_Renderer.PushGeometry(m_ScratchBuffer.data(), cmd.vertexCount, cmd.vertexSize, cmd.indexData, cmd.indexCount);
	}
	m_Renderer.Flush(m_State.layout);

	flushCounter++;
	flushCount = flushCounter;


	m_Commands.clear();
	m_State = {};
}