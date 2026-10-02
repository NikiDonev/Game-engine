#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include <glad/glad.h>

class TextureTable {
public:
	static constexpr int kShaderArraySize = 32;
	std::vector<uint32_t> textureSlots;

	void Init() {
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_MaxSlots);
		m_MaxSlots = std::min(m_MaxSlots, 32);
		textureSlots.reserve(m_MaxSlots);
	}

	void Clear() { textureSlots.clear(); }
	bool IsFull() const { return (int)textureSlots.size() >= m_MaxSlots; }
	int Count() const { return (int)textureSlots.size(); }
	int MaxSlots() { return m_MaxSlots; }

	int Find(uint32_t textureID) const {
		auto it = std::find(textureSlots.begin(), textureSlots.end(), textureID);
		return it == textureSlots.end() ? -1 : (int)std::distance(textureSlots.begin(), it);
	}
	int Add(uint32_t textureID) {      // slot index, or -1 when full
		int slot = Find(textureID);
		if (slot >= 0) return slot;
		if (IsFull()) return -1;
		textureSlots.push_back(textureID);
		return (int)textureSlots.size() - 1;
	}
	void BindAll() const {
		for (int i = 0; i < (int)textureSlots.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureSlots[i]);
		}
	}

private:
	int m_MaxSlots{};
};

