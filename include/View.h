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
	View(glm::mat4 matrix = glm::mat4(1.0f)) {

	}
	void setSize(int Width, int Height) {
		width = Width;
		height = Height;
	}
	glm::mat4 getViewMatrix() {
		glm::mat4 view = glm::mat4(1.0f);
		scale.x = std::max(0.0f, scale.x);
		scale.y = std::max(0.0f, scale.y);
		view = glm::translate(view, glm::vec3(pos, 0.0f));
		view = glm::translate(view, glm::vec3(origin, 0.0f));
		view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::scale(view, glm::vec3(scale, 1.0f));
		view = glm::translate(view, glm::vec3(-origin, 0.0f));
		return view;
	}
	glm::mat4 getProjectionMatrix() {
		return glm::ortho(-(float)width / 2.0f, (float)width / 2.0f, -(float)height / 2.0f, (float)height / 2.0f);
	}
	glm::mat4 getViewProjMatrix() {
		return getViewMatrix() * getProjectionMatrix();
	}

	operator glm::mat4() {
		return getViewMatrix();
	}
};