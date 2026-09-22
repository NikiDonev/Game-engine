#include "Input.h"
#include <iostream>
#include "DebugUI.h"

void Input::Update() {
	previousKeys = currentKeys;
	previousMouse = currentMouse; 


	for (int i = 0; i < joysticks.size(); ++i) {
		if (joysticks[i].active) {
			joysticks[i].QueryData(i);
		}
	}
}

void Input::ClearTextBuffer() {
	textBuffer.clear();
}

bool Input::KeyHeld(int keycode) {
	if (keycode < 0 || keycode >= 350) return false;
	return currentKeys[keycode];
}

bool Input::KeyPressed(int keycode) {
	if (keycode < 0 || keycode >= 350) return false;
	return currentKeys[keycode] && !previousKeys[keycode];
}

bool Input::KeyReleased(int keycode) {
	if (keycode < 0 || keycode >= 350) return false;
	return !currentKeys[keycode] && previousKeys[keycode];
}
bool Input::MouseHeld(int button) {
	if (button < 0 || button >= 8) return false;
	return currentMouse[button];
}

bool Input::MousePressed(int button) {
	if (button < 0 || button >= 8) return false;
	return currentMouse[button] && !previousMouse[button];
}

bool Input::MouseReleased(int button) {
	if (button < 0 || button >= 8) return false;
	return !currentMouse[button] && previousMouse[button];
}


glm::vec2 Input::getMousePixelPos() {
	return glm::vec2((float)mouseX, (float)mouseY);
}

glm::vec2 Input::pixelToWorld(glm::vec2 pixelCoords, const View& view) {
	int windowWidth = m_Width, windowHeight = m_Height;
	if (windowWidth == 0 || windowHeight == 0) return glm::vec2(0.0f);

	float ndcX = (2.0f * pixelCoords.x / windowWidth) - 1.0f;  	// Convert screen pixels to NDC
	float ndcY = 1.0f - (2.0f * pixelCoords.y / windowHeight);

	glm::vec4 ndcPos(ndcX, ndcY, 0.0f, 1.0f);
	glm::mat4 invViewProj = glm::inverse(view.getViewProjMatrix());

	glm::vec4 worldPos = invViewProj * ndcPos; // Unproject the coordinate into world space

	return glm::vec2(worldPos.x, worldPos.y);
}

glm::vec2 Input::getMouseWorldPos(const View& view) {
	return pixelToWorld(getMousePixelPos(), view);
}

glm::vec2 Input::getScroll() {
	return glm::vec2((float)scrollX, (float)scrollY);
}

void Input::CheckActiveJoysticks() {
	for (int i = 0; i < joysticks.size(); ++i) {
		joysticks[i].active = glfwJoystickPresent(GLFW_JOYSTICK_1 + i);
		joysticks[i].QueryData(i);
	}
}


void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (DebugUI::KeyCallback(window, key, scancode, action, mods)) return;

	if (key < 0 || key >= 350) return;

	if (action == GLFW_PRESS) currentKeys[key] = true;
	else if (action == GLFW_RELEASE) currentKeys[key] = false;
	
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	if (DebugUI::MouseCallback(window, button, action, mods)) return;

	if (button < 0 || button >= 8) return;

	if (action == GLFW_PRESS) currentMouse[button] = true;
	else if (action == GLFW_RELEASE) currentMouse[button] = false;
}

void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	mouseX = xpos;
	mouseY = ypos;
}

void Input::CharCallback(GLFWwindow* window, unsigned int codepoint) {
	if (codepoint < 128) {
		textBuffer += static_cast<char>(codepoint);
	}
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	if (DebugUI::ScrollCallback(window, xoffset, yoffset)) return;
	// TODO: fix scroll by finding the delta
	scrollX += xoffset;
	scrollY += yoffset;
}

void Input::WindowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	m_Width = width;
	m_Height = height;
}

void Input::JoystickCallback(int jid, int event) {
	if (event == GLFW_CONNECTED) {
		joysticks[jid].active = true;
		joysticks[jid].QueryData(jid);
	}
	else {
		joysticks[jid] = Joystick();
	}
}

void Input::SetupCallbacks(GLFWwindow* window) {
	    glfwSetWindowUserPointer(window, this);

    // C-compatible Lambdas
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (input) input->KeyCallback(win, key, scancode, action, mods);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (input) input->MouseButtonCallback(win, button, action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (input) input->CursorPosCallback(win, xpos, ypos);
    });

    glfwSetCharCallback(window, [](GLFWwindow* win, unsigned int codepoint) {
        auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (input) input->CharCallback(win, codepoint);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
        auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (input) input->ScrollCallback(win, xoffset, yoffset);
    });
	
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
		if (input) input->WindowSizeCallback(win, width, height);
		});


    s_Instance = this; 
    glfwSetJoystickCallback([](int jid, int event) {
        if (s_Instance) s_Instance->JoystickCallback(jid, event);
    });

	CheckActiveJoysticks();
}
