#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "openglErrorReporting.h"
#include "shader.h"

struct State {
	bool depthTesting = false;
	bool stencilTesting = false;
	bool faceCulling = false;
	bool blending = false;
	GLenum blendFactorS = GL_SRC_ALPHA, blendFactorD = GL_ONE_MINUS_SRC_ALPHA;
	uint32_t vao{};
	GLenum primitive = GL_TRIANGLES;
	uint32_t shader{};
	uint32_t texture{};

};

class StateManager {
public:
	State state;

	void setShader(const Shader& shader) {
		if (state.shader.ID != shader.ID) {
			state.shader.use();
			state.shader = shader;
		}
	}
	void setTexture(uint32_t texture, GLenum textureTarget = GL_TEXTURE_2D) {
		if (state.texture != texture) {
			glBindTexture(textureTarget, texture);
			state.texture = texture;
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
	void switchState (const State& newState) {
		setShader(newState.shader);
		setTexture(newState.texture);
		setDepthTesting(newState.depthTesting);
		setStencilTesting(newState.stencilTesting);
		setFaceCulling(newState.faceCulling);
	}
private:

};