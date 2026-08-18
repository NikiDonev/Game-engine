#pragma once
#include <stb_image/stb_image.h>

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>


class Texture {
public:
	Texture(const std::string& filePath, GLint wrap = GL_REPEAT, GLint filter = GL_NEAREST);
	Texture(const std::string& virtualPath, uint8_t* data, int width, int height, int channels, GLint wrap = GL_REPEAT, GLint filter = GL_NEAREST);

	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	uint32_t GetID() const { return m_ID; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	bool operator==(const Texture& other) const { return m_ID == other.m_ID; }
	bool operator!=(const Texture& other) const { return m_ID != other.m_ID; }

	static std::shared_ptr<Texture> GetWhiteTexture();
	static std::shared_ptr<Texture> GetErrorTexture();

private:
	void Swap(Texture& other);

	void LoadFromFile(const char* path);
	void LoadFromData(uint8_t* data, int channels);
	void setFormats(int channels);

	std::string m_Path;
	uint32_t m_ID{};
	int m_Width{}, m_Height{};
	GLint m_Wrap{GL_REPEAT};
	GLint m_Filter{ GL_NEAREST };
	GLenum m_DataFormat{ GL_RGB };
	GLenum m_InternalFormat{ GL_RGB8 };
	GLenum m_TextureType{ GL_TEXTURE_2D };

};