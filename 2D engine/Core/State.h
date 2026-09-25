#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "openglErrorReporting.h"



struct State {
	bool depthTesting = false;
	bool stencilTesting = false;
	bool faceCulling = false;
	bool blending = false;
	GLenum blendFactorS = GL_SRC_ALPHA, blendFactorD = GL_ONE_MINUS_SRC_ALPHA;
	uint32_t VAO{};
	uint32_t shaderID{};

	
	std::array<uint32_t, 32> boundTextures{ 0 };
	uint32_t activeTextureUnit = 0;
};

class StateManager {
public:
	State state;

	void setShader(uint32_t shaderID) {
		if (state.shaderID != shaderID) {
			glUseProgram(shaderID);
			state.shaderID = shaderID;
		}
	}
	void setTexture(uint32_t unit, uint32_t textureID, GLenum textureTarget = GL_TEXTURE_2D) {
		if (state.activeTextureUnit != unit) {
			glActiveTexture(GL_TEXTURE0 + unit);
			state.activeTextureUnit = unit;
		}
		if (state.boundTextures[unit] != textureID) {
			glBindTexture(textureTarget, textureID);
			state.boundTextures[unit] = textureID;
		}
	}
	void setVAO(uint32_t VAO) {
		if (state.VAO != VAO) {
			glBindVertexArray(VAO);
			state.VAO = VAO;
		}
	}
	void setDepthTesting(bool depthTest) {
		if (state.depthTesting != depthTest) {
			if (depthTest) glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
			state.depthTesting = depthTest;
		}
	}
	void setStencilTesting(bool stencilTest) {
		if (state.stencilTesting != stencilTest) {
			if (stencilTest) glEnable(GL_STENCIL_TEST);
			else glDisable(GL_STENCIL_TEST);
			state.stencilTesting = stencilTest;
		}
	}
	void setFaceCulling(bool faceCulling) {
		if (state.faceCulling != faceCulling) {
			state.faceCulling = faceCulling;
			if (faceCulling) glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
		}
	}
	void setBlending(bool enabled, GLenum src = GL_SRC_ALPHA, GLenum dst = GL_ONE_MINUS_SRC_ALPHA) {
		if (state.blending != enabled) {
			if (enabled) glEnable(GL_BLEND);
			else glDisable(GL_BLEND);
			state.blending = enabled;
		}

		if (enabled) {
			glBlendFunc(src, dst);
		}
	}
	void switchState (const State& newState) {
		setShader(newState.shaderID);
		for (int i = 0; i < state.boundTextures.size(); ++i) 
			setTexture(i, newState.boundTextures[i]);
		
		setDepthTesting(newState.depthTesting);
		setStencilTesting(newState.stencilTesting);
		setFaceCulling(newState.faceCulling);
	}
private:

};