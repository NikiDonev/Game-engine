#include "TextureManager.h"
#include "TextureManager.h"


TextureManager::~TextureManager() {
	for (auto& pair : textures) {
		glDeleteTextures(1, &pair.second);
	}
	for (unsigned int id : createdTextures) {
		glDeleteTextures(1, &id);
	}
	textures.clear();
}
unsigned int TextureManager::componentsToFormat(unsigned int components) {
	GLenum format{};
	if (components == 1)
		format = GL_RED;
	else if (components == 2)
		format = GL_RG;
	else if (components == 3)
		format = GL_RGB;
	else if (components == 4)
		format = GL_RGBA;
	return format;
}

unsigned int TextureManager::loadTexture(char const* path, bool flip, 
	GLint swrap, GLint twrap, GLint minFilter, GLint magFilter) {
	auto it = textures.find(path);
	if (it != textures.end())
		return it->second;
	
	stbi_set_flip_vertically_on_load(flip);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = componentsToFormat(nrComponents);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	textures[path] = textureID;
	return textureID;
}

unsigned int TextureManager::createTexture(unsigned int width, unsigned int height, 
	GLenum format, GLint swrap, GLint twrap, GLint minFilter, GLint magFilter) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twrap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	bool minFilterNeedsMips = (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_LINEAR_MIPMAP_LINEAR);
	if (minFilterNeedsMips) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		// Ensure only level 0 is used so the texture is complete without mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);	
	createdTextures.push_back(textureID);
	return textureID;
}


unsigned int TextureManager::loadCubemap(std::vector<std::string> faces) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = componentsToFormat(nrChannels);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
