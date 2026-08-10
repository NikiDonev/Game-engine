#include "Application.h"
#include "shader.h"
#include "rect.h"
#include "TextureManager.h"
#include "Renderer2D.h"
#include "Input.h"
#include "SpriteRenderer.h"

int WIDTH{ 800 }, HEIGHT{ 600 };
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = nullptr;

float cooldown = 0.0f;
View view;

bool mPressed = false;
bool showDebug = true;
Sprite s1, s2;
Renderer2D renderer;
SpriteRenderer spriteRenderer;
uint32_t mossTexture, deepslateTexture;

std::vector<Sprite> sprites;

void processInput(GLFWwindow* window, float deltaTime) {
	if (Input::KeyHeld(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	if (Input::KeyPressed(GLFW_KEY_M)) showDebug = !showDebug;

	if (Input::KeyHeld(GLFW_KEY_W)) s1.transform.pos.y += 5 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_S)) s1.transform.pos.y -= 5 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_D)) s1.transform.pos.x += 5 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_A)) s1.transform.pos.x -= 5 * deltaTime;


	if (Input::KeyHeld(GLFW_KEY_UP)) view.pos.y -= 5 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_DOWN)) view.pos.y += 5 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_RIGHT)) view.pos.x -= 5 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_LEFT)) view.pos.x += 5 * deltaTime;

	if (Input::KeyHeld(GLFW_KEY_Z)) view.scale += 30 * deltaTime;
	if (Input::KeyHeld(GLFW_KEY_X)) view.scale -= 30 * deltaTime;



	if (Input::KeyPressed(GLFW_KEY_ENTER)) {
		Input::ClearTextBuffer();
	}
	//if (Input::joysticks[0].active) {
	//	return;
	//	const auto& axes = Input::joysticks[0].axes;
	//	const auto& buttons = Input::joysticks[0].buttons;
	//	//if (buttons[7]) showDebug = !showDebug;
	//	if (buttons[8]) glfwSetWindowShouldClose(window, true);
	//	//if (buttons[1]) s1.rotation -= 200 * deltaTime;
	//	if (buttons[3]) view.scale -= 30 * deltaTime;
	//	if (buttons[4]) view.scale += 30 * deltaTime
	//	const float viewSpeed = 300;
	//	if (buttons[18]) view.pos.x += viewSpeed * deltaTime;
	//	if (buttons[16]) view.pos.x -= viewSpeed * deltaTime;
	//	if (buttons[17]) view.pos.y += viewSpeed * deltaTime;
	//	if (buttons[15]) view.pos.y -= viewSpeed * deltaTime;
	//	float speed = 5.0f;
	//	unsigned char pressed = buttons[13] | buttons[14] | buttons[15] | buttons[16] | buttons[17];
	//	const float waitTime = 1.2f, dashTime = 0.1f;
	//	if (buttons[0]) {
	//		if (cooldown > waitTime + dashTime) {
	//			cooldown = 0.0f;
	//		}
	//		if (cooldown < dashTime) {
	//			speed = 40.0f;
	//		}
	//		
	//	}
	//	if(cooldown < waitTime + dashTime)
	//		cooldown += deltaTime;
	//	
	//	s1.transform.pos.x += speed * axes[0] * deltaTime;
	//	s1.transform.pos.y -= speed * axes[1] * deltaTime;
	//}

}
void OnRender() {

}
char textBuffer[512];
void ImguiDebug() {
	ImGui::Text("FPS: %f", 1.0f / deltaTime);
	ImGui::Text("Cooldown: %f", cooldown);
	ImGui::Text("Input text: %s", Input::getTextBuffer().c_str());
	ImGui::Text("Cube pos: %f, %f", s1.transform.pos.x, s1.transform.pos.y);
	ImGui::Text("Scroll: %f", Input::getScroll().y);
	for (int i = 0; i < 8; ++i) {
		if (Input::MouseHeld(GLFW_MOUSE_BUTTON_1 + i)) {
			ImGui::Text("Mouse button %i pressed", i+1);
		}
	}


	//ImGui::Text("Joystick 1 active: %d", Input::joysticks[0].active);
	//const auto& axes = Input::joysticks[0].axes;
	//const auto& buttons = Input::joysticks[0].buttons;
	//const auto& hats = Input::joysticks[0].hats;
	//for (const auto& axis : axes) 
	//	ImGui::Text("Axis value: %f ", axis);
	//for (const auto& button : buttons) 
	//	ImGui::Text("Button value: %d ", button);
	//ImGui::InputText("Some random text", textBuffer, 512);
}

void OnInit()  {
	renderer.Init();
	spriteRenderer.Init();

	view.scale = glm::vec2(80.0f);

	s1.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	s2.color = { 0.0f, 1.0f, 1.0f, 1.0f };

	mossTexture = TextureManager::Get().loadTexture(RESOURCES_PATH "moss.png");
	deepslateTexture = TextureManager::Get().loadTexture(RESOURCES_PATH "deepslate.png");

	s1.textureID = mossTexture;
	s2.textureID = deepslateTexture;

	sprites.resize(1000);
	for (int i = 0; i < sprites.size(); ++i) {
		sprites[i].textureID = mossTexture;
		sprites[i].transform.pos.x = (i % 10) * 2;
		sprites[i].transform.pos.y = (i / 10) * 2;

		//sprites[i].transform.rotation = 45.0f;
		int number = (i) % 100 + 3;
		std::string name = "Screenshot (" + std::to_string(number) + ").png";
		std::string filepath = RESOURCES_PATH + name;
		sprites[i].textureID = mossTexture;
		//sprites[i].textureID = TextureManager::Get().loadTexture(filepath.c_str(), true);
	}
}

void OnUpdate(float deltaTime) {

	glfwGetWindowSize(window, &WIDTH, &HEIGHT);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//renderer.Add(s1);

	s2.transform.pos.x = 3 * sin(glfwGetTime());
	s2.transform.pos.y = 3 * cos(glfwGetTime());

	spriteRenderer.Add(s2);
	spriteRenderer.Add(s1);
	for (int i = 0; i < sprites.size(); ++i) {
		//renderer.Add(sprites[i]);
	}

	view.setSize(WIDTH, HEIGHT);
	renderer.setView(view);
	spriteRenderer.setView(view);

	//renderer.DrawSprites();
	spriteRenderer.DrawSprites();

}

int main() {
	window = InitializeOpenGL(WIDTH, HEIGHT);
	Input::SetupCallbacks(window);

	OnInit();

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::Update();


		OnUpdate(deltaTime);
		processInput(window, deltaTime);

		if (showDebug) {
			ImGuiBegin();
			ImguiDebug();
			ImGuiEnd();
		}
		glfwSwapBuffers(window);
	}

	DeinitializeOpenGL();
}