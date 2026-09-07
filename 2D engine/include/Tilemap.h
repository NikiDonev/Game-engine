#include <vector>
#include <iostream>
#include <unordered_map>
#include "TextureAtlas.h"
#include <glm/glm.hpp>
#include "SpriteRenderer.h"


struct PairHash {
	std::size_t operator()(const std::pair<int, int>& p) const {
		// A simple, classic spatial hash combination that works perfectly with negative numbers
		std::size_t h1 = std::hash<int>{}(p.first);
		std::size_t h2 = std::hash<int>{}(p.second);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
	}
};

class TilemapChunk {
public:
	static const int Size = 16;
	std::vector<int> grid;
	TilemapChunk() {
		grid.resize(Size * Size, -1);

	}
	void SetTile(glm::ivec2 tile, int atlasIndex) {
		if (tile.x >= 0 && tile.x < Size && tile.y >= 0 && tile.y < Size) {
			grid[tile.x + Size * tile.y] = atlasIndex;
		}
	}

private:

};


class Tilemap : Transformable{
public:
	Tilemap(const Ref<Texture>& tileset, const TextureAtlas& atlas, float tileSize) : m_Tileset(tileset), m_Atlas(atlas), m_TileSize(tileSize){
		m_ChunkSize = m_TileSize * TilemapChunk::Size;
	}

	void SetTile(glm::ivec2 tilepos, int atlasIndex) {
		int cx = static_cast<int>(std::floor(static_cast<float>(tilepos.x) / 16.0f));
		int cy = static_cast<int>(std::floor(static_cast<float>(tilepos.y) / 16.0f));

		int lx = tilepos.x - (cx * 16);
		int ly = tilepos.y - (cy * 16);

		auto& chunk = m_Chunks[{cx, cy}];
		chunk.SetTile({ lx, ly }, atlasIndex);
	}

	void Draw(SpriteRenderer& renderer) {
		const AABB& viewFrustum = renderer.view.getFrustumBounds();
		const auto& model = getTransformMatrix();

		for (const auto& [key, chunk] : m_Chunks) {
			glm::ivec2 chunkCoords = { key.first, key.second };
			glm::vec2 chunkPos = (float)m_ChunkSize * (glm::vec2)chunkCoords;
			DrawChunk(chunk, renderer, chunkPos, model, viewFrustum);

		}
	}

	void DrawChunk(const TilemapChunk& chunk, SpriteRenderer& renderer, glm::vec2 chunkPos, const glm::mat4& model, const AABB& viewFrustum) {

		if (chunkPos.x + m_ChunkSize < viewFrustum.min.x || chunkPos.x > viewFrustum.max.x ||
			chunkPos.y + m_ChunkSize < viewFrustum.min.y || chunkPos.y > viewFrustum.max.y) {
			return;
		}
		uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
		glm::vec2 xAxis = glm::vec2(model * glm::vec4(m_TileSize, 0.0f, 0.0f, 0.0f));
		glm::vec2 yAxis = glm::vec2(model * glm::vec4(0.0f, m_TileSize, 0.0f, 0.0f));

		glm::vec2 chunkOrigin = glm::vec2(model * glm::vec4(chunkPos.x, chunkPos.y, 0.0f, 1.0f));

		for (int ly = 0; ly < TilemapChunk::Size; ++ly) {
			for (int lx = 0; lx < TilemapChunk::Size; ++lx) {
				int tileIndex = chunk.grid[ly * TilemapChunk::Size + lx];
				if (tileIndex == -1) continue;

				glm::vec4 uvs = m_Atlas.getTexCoords(tileIndex, 0.0f);

				float px = chunkPos.x + (lx * m_TileSize);
				float py = chunkPos.y + (ly * m_TileSize);
				glm::vec4 white = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

				glm::vec2 tileOrigin = chunkOrigin + (static_cast<float>(lx) * xAxis) + (static_cast<float>(ly) * yAxis);
				glm::vec2 positions[4] = {
					tileOrigin,
					tileOrigin + xAxis,
					tileOrigin + xAxis + yAxis,
					tileOrigin + yAxis
				};
				uint32_t slot = renderer.getTextureSlot(m_Tileset);

				SpriteVertex vertices[4] = {
					{ positions[0], white, { uvs.x, uvs.y }, slot },
					{ positions[1], white, { uvs.z, uvs.y }, slot },
					{ positions[2], white, { uvs.z, uvs.w }, slot },
					{ positions[3], white, { uvs.x, uvs.w }, slot }
				};

				renderer.renderer.PushGeometry(vertices, 4, indices, 6);
			}
		}
	}

private:
	const TextureAtlas& m_Atlas;
	Ref<Texture> m_Tileset;
	float m_TileSize{ }, m_ChunkSize{};
	std::unordered_map<std::pair<int,int>, TilemapChunk, PairHash> m_Chunks;
};