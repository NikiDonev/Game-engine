#include "Texture.h"


Texture::Texture(const std::string& filePath, GLint wrap, GLint filter)
	: m_Path(filePath), m_Wrap(wrap), m_Filter(filter) {
	LoadFromFile(filePath.c_str());
}

Texture::Texture(const std::string& virtualPath, uint8_t* data, int width, int height, int channels, GLint wrap, GLint filter)
	: m_Path(virtualPath), m_Width(width), m_Height(height), m_Wrap(wrap), m_Filter(filter) {
	LoadFromData(data, channels);
}

Texture::~Texture() {
	if (m_ID != 0) {
		std::cout << "Deleting texture " << m_ID << std::endl;
		glDeleteTextures(1, &m_ID);
	}
}

Texture::Texture(Texture&& other) noexcept {
	Swap(other);
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		if (m_ID != 0) {
			glDeleteTextures(1, &m_ID);
			m_ID = 0;
		}
		Swap(other);
	}
	return *this;
}

std::shared_ptr<Texture> Texture::GetWhiteTexture() {
	std::shared_ptr<Texture> s_WhiteTexture = nullptr;
	if (!s_WhiteTexture) {
		uint8_t data[4] = { 255, 255, 255, 255 };
		s_WhiteTexture = std::make_shared<Texture>("runtime://white_texture", data, 1, 1, 4, GL_REPEAT, GL_NEAREST);
	}
	return s_WhiteTexture;
}

std::shared_ptr<Texture> Texture::GetErrorTexture() {
	std::shared_ptr<Texture> s_ErrorTexture = nullptr;
	if (!s_ErrorTexture) {
		uint8_t data[16] = { 
			0  , 0  , 0  , 255, 
			255, 0  , 255, 255, 
			255, 0  , 255, 255, 
			0  , 0  , 0  , 255 
		};
		s_ErrorTexture = std::make_shared<Texture>("runtime://error_texture", data, 2, 2, 4, GL_CLAMP_TO_EDGE, GL_NEAREST);
	}
	return s_ErrorTexture;
}

void Texture::Swap(Texture& other) {
	std::swap(m_ID, other.m_ID);
	std::swap(m_Path, other.m_Path);
	std::swap(m_Width, other.m_Width);
	std::swap(m_Height, other.m_Height);
	std::swap(m_Wrap, other.m_Wrap);
	std::swap(m_Filter, other.m_Filter);
	std::swap(m_DataFormat, other.m_DataFormat);
	std::swap(m_InternalFormat, other.m_InternalFormat);
	std::swap(m_TextureType, other.m_TextureType);
}


void Texture::LoadFromFile(const char* path) {
	stbi_set_flip_vertically_on_load(true);

	int channels;
	uint8_t* data = stbi_load(path, &m_Width, &m_Height, &channels, 0);
	if (data) {
		LoadFromData(data, channels);
		stbi_image_free(data);
	}
	else {
		std::cout << "[Texture Error] Failed to load texture at path: " << path << std::endl;
		uint8_t errorTexture[16] = {
			0  , 0  , 0  , 255,
			255, 0  , 255, 255,
			255, 0  , 255, 255,
			0  , 0  , 0  , 255
		};

		m_Width = 2;
		m_Height = 2;
		channels = 4;

		m_Wrap = GL_CLAMP_TO_EDGE;
		LoadFromData(errorTexture, channels);
		return;
	}
}


void Texture::LoadFromData(uint8_t* data, int channels) {
	glGenTextures(1, &m_ID);

	setFormats(channels);

	glBindTexture(m_TextureType, m_ID);
	glTexImage2D(m_TextureType, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(m_TextureType);

	glTexParameteri(m_TextureType, GL_TEXTURE_WRAP_S, m_Wrap);
	glTexParameteri(m_TextureType, GL_TEXTURE_WRAP_T, m_Wrap);

	GLint minFilter = m_Filter;
	if (m_Filter == GL_NEAREST) minFilter = GL_NEAREST_MIPMAP_NEAREST;
	else if (m_Filter == GL_LINEAR) minFilter = GL_LINEAR_MIPMAP_LINEAR;

	glTexParameteri(m_TextureType, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(m_TextureType, GL_TEXTURE_MAG_FILTER, m_Filter);

	glBindTexture(m_TextureType, 0);
}

void Texture::setFormats(int channels) {
	if (channels == 4) {
		m_InternalFormat = GL_RGBA8; m_DataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		m_InternalFormat = GL_RGB8;  m_DataFormat = GL_RGB;
	}
	else if (channels == 2) {
		m_InternalFormat = GL_RG8;   m_DataFormat = GL_RG;
	}
	else if (channels == 1) {
		m_InternalFormat = GL_R8;    m_DataFormat = GL_RED;
	}
}

void Texture::Bind(uint32_t slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(m_TextureType, m_ID);
}

void Texture::Unbind() const {
	glBindTexture(m_TextureType, 0);
}