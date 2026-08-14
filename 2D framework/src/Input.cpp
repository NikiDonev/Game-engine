#include "Input.h"



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


glm::vec2 Input::getMousePos() {
	return glm::vec2((float)mouseX, (float)mouseY);
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
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard) return;

	if (key < 0 || key >= 350) return;

	if (action == GLFW_PRESS) currentKeys[key] = true;
	else if (action == GLFW_RELEASE) currentKeys[key] = false;
	
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;

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
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) {
		return;
	}
	// TODO: fix scroll by finding the delta
	scrollX += xoffset;
	scrollY += yoffset;
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
	
	//glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
	//	auto* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
	//	if (input) input->CursorPosCallback(win, xpos, ypos);
	//	});


    s_Instance = this; 
    glfwSetJoystickCallback([](int jid, int event) {
        if (s_Instance) s_Instance->JoystickCallback(jid, event);
    });

	CheckActiveJoysticks();
}
