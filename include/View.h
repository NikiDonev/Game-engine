#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class View {
public:
	glm::vec2 pos{}, origin{};
	float rotation{};
	int width{}, height{};
	glm::vec2 scale = glm::vec2(1.0f);

	View() = default;
	View(int w, int h) : width(w), height(h) {}

	void setSize(int Width, int Height) {
		width = Width;
		height = Height;
	}
	glm::mat4 getViewMatrix() const {
		glm::mat4 cameraTransform = glm::mat4(1.0f);

		//scale.x = std::max(0.001f, scale.x);
		//scale.y = std::max(0.001f, scale.y);

		cameraTransform = glm::translate(cameraTransform, glm::vec3(pos, 0.0f));
		cameraTransform = glm::translate(cameraTransform, glm::vec3(origin, 0.0f));
		cameraTransform = glm::rotate(cameraTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		cameraTransform = glm::scale(cameraTransform, glm::vec3(scale, 1.0f));
		cameraTransform = glm::translate(cameraTransform, glm::vec3(-origin, 0.0f));
		return cameraTransform;
		//return glm::inverse(cameraTransform);
	}
	glm::mat4 getProjectionMatrix() const {
		return glm::ortho(-(float)width / 2.0f, (float)width / 2.0f, -(float)height / 2.0f, (float)height / 2.0f);
	}
	glm::mat4 getViewProjMatrix() const {
		return getProjectionMatrix()* getViewMatrix();
	}

	operator glm::mat4() {
		return getViewMatrix();
	}
};