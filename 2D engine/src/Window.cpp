#include "Window.h"


void Window::Init(uint32_t width, uint32_t height, const char* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (glfwWindow == NULL) {
		std::cerr << "Failed to create a window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(glfwWindow);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize glad" << std::endl;
		return;
	}

	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);

	DebugUI::Init(glfwWindow);

	enableReportGlErrors();
}

Window::~Window() {
	if (glfwWindow) {
		glfwDestroyWindow(glfwWindow);
	}
	DebugUI::Shutdown();

	glfwTerminate();
}


