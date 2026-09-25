#include "DebugUI.h"
#include "Logging.h"

#if PRODUCTION_BUILD
	void DebugUI::Init(GLFWwindow* glfwWindow) {}
	void DebugUI::Shutdown() {}
	void DebugUI::BeginFrame() {}
	void DebugUI::EndFrame() {}
	void DebugUI::ShowLog() {}
	bool DebugUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { return false; }
	bool DebugUI::MouseCallback(GLFWwindow* window, int button, int action, int mods) { return false; }
	bool DebugUI::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) { return false; }
	//void DebugUI::ShowProfiler(float* frameHistory, int historyIdx = 0, float deltaTime);
#else
	void DebugUI::Init(GLFWwindow* glfwWindow) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();


		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, false);
		ImGui_ImplOpenGL3_Init("#version 330");
	}
	void DebugUI::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void DebugUI::BeginFrame(){
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(
			ImGui::GetMainViewport(),
			ImGuiDockNodeFlags_PassthruCentralNode);
	}
	void DebugUI::EndFrame(){
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void DebugUI::ShowLog(){
		DRAW_LOG();
	}
	bool DebugUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureKeyboard;
	}
	bool DebugUI::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureMouse;
	}
	bool DebugUI::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

		ImGuiIO& io = ImGui::GetIO();

		return io.WantCaptureMouse;

	}
	//void DebugUI::ShowProfiler(float* frameHistory, int historyIdx = 0, float deltaTime) {}
#endif