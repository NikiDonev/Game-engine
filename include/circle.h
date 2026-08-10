#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Circle {
public:

	unsigned int VBO, VAO;
	float radius = 1.0f;
	std::vector<float>points{};
	int vertCount{};
	float angle{};
	Circle(float r, unsigned int n = 100) : radius(r), vertCount(n+1) {

		points.resize(5 * n + 5);

		points[0] = points[1] = points[2] = 0.0f;
		points[3] = points[4] = 0.5f;
		float angleDelta = 360 / (float)(n-1), angle = 0.f;
		for (int i = 1; i <= n; ++i) {
			points[5 * i] = cos(glm::radians(angle));
			points[5 * i + 1] = sin(glm::radians(angle));
			points[5 * i + 2] = 0.0f;
			points[5 * i + 3] = (points[5 * i] / 2.0f) + 0.5f;
			points[5 * i + 4] = (points[5 * i + 1] / 2.0f) + 0.5f;
			angle += angleDelta;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER , VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	void draw(Shader& shader) {
		glm::mat4 trans = glm::mat4(1.0f);
		angle += 0.01f;
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.333f, 0.333f, 0.334f));
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(radius, radius, 1.0f));
		unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertCount);
	}

};