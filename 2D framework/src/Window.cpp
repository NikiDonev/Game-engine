#include "Window.h"

//void frame_buffer_size_callback(GLFWwindow* window, int Width, int Height) {
//	glViewport(0, 0, Width, Height);
//	width = Width;
//	height = Height;
//}


void ImGuiBegin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Debug");
}

void ImGuiEnd() {
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}