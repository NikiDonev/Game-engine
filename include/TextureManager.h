#pragma once

#include <stb_image/stb_image.h>

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

class TextureManager {
public:
	~TextureManager();

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	static TextureManager& Get() {
		static TextureManager instance;
		return instance;
	}

	unsigned int loadTexture(const char* path, bool flip = false,
		GLint swrap = GL_REPEAT, GLint twrap = GL_REPEAT, GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);

	unsigned int createTexture(unsigned int width, unsigned int height, GLenum format,
		GLint swrap = GL_REPEAT, GLint twrap = GL_REPEAT, GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);

	unsigned int loadCubemap(std::vector<std::string> faces);

	unsigned int componentsToFormat(unsigned int components);

private:
	TextureManager() = default;

	std::unordered_map<std::string, unsigned int> textures;
	std::vector<unsigned int> createdTextures;
};