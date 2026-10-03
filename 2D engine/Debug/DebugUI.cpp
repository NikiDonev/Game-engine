#include "DebugUI.h"
#include "Logging.h"
#include "Instrumentor.h"

#if !DEBUG_UI
	void DebugUI::Init(GLFWwindow* glfwWindow) {}
	void DebugUI::ApplyStyle() {}
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
		PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();


		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, false);
		ImGui_ImplOpenGL3_Init("#version 330");
	}
	void DebugUI::ApplyStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// --- Base Surfaces & Backgrounds ---
		colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f); // Deep charcoal-slate
		colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.08f, 0.95f);

		// --- Frames (Input boxes, checkboxes, sliders) ---
		colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.24f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.34f, 0.59f, 0.40f); // Subtle blue tint when active

		// --- Title Bars & Windows Headers ---
		colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);

		// --- Tabs (Beautifully integrated for the Docking Branch) ---
		colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.12f, 1.00f); // Matches inactive window titles
		colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.22f, 0.26f, 1.00f); // Subtle highlight
		colors[ImGuiCol_TabActive] = ImVec4(0.15f, 0.35f, 0.65f, 1.00f); // Muted focus blue
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.15f, 0.17f, 1.00f);

		// --- Buttons & Main Interactive Elements ---
		colors[ImGuiCol_Button] = ImVec4(0.16f, 0.36f, 0.64f, 0.80f); // Deep premium blue
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.50f, 1.00f, 0.90f); // Vibrant electric blue hover
		colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.40f, 0.85f, 1.00f);

		// --- Tree Nodes, List Selections, & Headers ---
		colors[ImGuiCol_Header] = ImVec4(0.16f, 0.36f, 0.64f, 0.45f); // Semi-transparent blue selection
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.50f, 1.00f, 0.60f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.17f, 0.50f, 1.00f, 0.80f);

		// --- Sliders & Grabs ---
		colors[ImGuiCol_SliderGrab] = ImVec4(0.17f, 0.50f, 1.00f, 0.70f); // Electric blue handle
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.17f, 0.50f, 1.00f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.17f, 0.50f, 1.00f, 1.00f); // Sharp blue check

		// --- Docking Previews & Layout Splits ---
		colors[ImGuiCol_DockingPreview] = ImVec4(0.17f, 0.50f, 1.00f, 0.45f); // Blue tint when dragging zones
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);

		// --- Clean Structural Settings ---
		style.WindowRounding = 3.0f; // Minimal, soft roundness for modern hardware styling
		style.ChildRounding = 3.0f;
		style.FrameRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.GrabRounding = 3.0f;

		style.WindowBorderSize = 1.0f;
		colors[ImGuiCol_Border] = ImVec4(0.18f, 0.20f, 0.23f, 0.60f); // Deep slate window outlines
		style.FrameBorderSize = 0.0f;
		style.PopupBorderSize = 1.0f;
	}
	void DebugUI::Shutdown() {
		PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void DebugUI::BeginFrame(){
		PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(
			ImGui::GetMainViewport(),
			ImGuiDockNodeFlags_PassthruCentralNode);
	}
	void DebugUI::EndFrame(){
		PROFILE_FUNCTION();
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