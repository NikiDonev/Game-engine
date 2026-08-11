#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct Joystick {
	
	
	void QueryData(int index){
		if (index < 0 || index >= 16) return;

		int joystick = GLFW_JOYSTICK_1 + index;
		active = true;
		int axisCount;
		const float* axesData = glfwGetJoystickAxes(joystick, &axisCount);
		axes.assign(axesData, axesData + axisCount);

		int buttonCount;
		const unsigned char* buttonsData = glfwGetJoystickButtons(joystick, &buttonCount);
		buttons.assign(buttonsData, buttonsData + buttonCount);

		int hatCount;
		const unsigned char* hatsData = glfwGetJoystickButtons(joystick, &hatCount);
		buttons.assign(hatsData, hatsData + hatCount);
	}
	bool active{ false };
	std::vector<float> axes;
	std::vector<unsigned char> buttons;
	std::vector<unsigned char> hats;
};


class Input {
public:
		
	void Update();
	void ClearTextBuffer();
	const std::string& getTextBuffer() { return textBuffer; };

	void resetScroll() { scrollX = 0.0; scrollY = 0.0; }

	bool KeyHeld(int keycode);
	bool KeyPressed(int keycode);
	bool KeyReleased(int keycode);

	bool MouseHeld(int button);
	bool MousePressed(int button);
	bool MouseReleased(int button);

	glm::vec2 getMousePos();
	glm::vec2 getScroll();
	
	void CheckActiveJoysticks();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void CharCallback(GLFWwindow* window, unsigned int codepoint);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void JoystickCallback(int jid, int event);
	
	void SetupCallbacks(GLFWwindow* window);

	std::array<Joystick, 16> joysticks{};

private:

	double mouseX{}, mouseY{};
	double scrollX{}, scrollY{};
	std::string textBuffer{};
	std::array<bool, 350> currentKeys{ false };
	std::array<bool, 350> previousKeys{ false };

	std::array<bool, 8> currentMouse{ false };
	std::array<bool, 8> previousMouse{ false };

	static inline Input* s_Instance = nullptr;
};
