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
		
	static void Update();
	static void ClearTextBuffer();
	static const std::string& getTextBuffer() { return textBuffer; };

	static void resetScroll() { scrollX = 0.0; scrollY = 0.0; }

	static bool KeyHeld(int keycode);
	static bool KeyPressed(int keycode);
	static bool KeyReleased(int keycode);

	static bool MouseHeld(int button);
	static bool MousePressed(int button);
	static bool MouseReleased(int button);

	static glm::vec2 getMousePos();
	static glm::vec2 getScroll();

	static void CheckActiveJoysticks();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void CharCallback(GLFWwindow* window, unsigned int codepoint);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static void JoystickCallback(int jid, int event);
	
	static void SetupCallbacks(GLFWwindow* window);

	static std::array<Joystick, 16> joysticks;
private:
	
	static double mouseX, mouseY;
	static double scrollX, scrollY;
	static std::string textBuffer;
	static std::array<bool, 350> currentKeys;
	static std::array<bool, 350> previousKeys;

	static std::array<bool, 8> currentMouse;
	static std::array<bool, 8> previousMouse;



};