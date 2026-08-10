#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Camera {
public:
	float yaw = 0.f, pitch = 0.0f;
	bool firstMouse = true;
	float fov = 45.0f;
	float lastX = 0.0f, lastY = 0.0f;

	glm::vec3 Position;
	glm::vec3 Front = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), float Yaw = 0.0f, float Pitch = 0.0f) 
		: Position(position), yaw(Yaw), pitch(Pitch) {
		
	}
	glm::mat4 getView() const{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processKeyboardInput(GLFWwindow* window, float deltaTime, float speed = 6.f) {
		float cameraSpeed = speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			cameraSpeed *= 2.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Position += Front * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= Front * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			Position -= Up * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			Position += Up * cameraSpeed;
	}
	void rotate(GLFWwindow* window, double xpos, double ypos) {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		pitch = std::clamp(pitch, -89.0f, 89.0f);

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		Front = glm::normalize(direction);
	}

	void zoom(GLFWwindow* window, double xoffset, double yoffset) {
		fov -= (float)yoffset;
		fov = std::clamp(fov, 1.0f, 90.0f);
	}
};


