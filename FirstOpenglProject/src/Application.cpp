#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "InputManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

const GLint WIDTH = 960, HEIGHT = 540;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;
VertexBuffer* vb = nullptr;
IndexBuffer* ib = nullptr;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

// Size
bool sizeDir = true;
float curSize = 0.4f;
float maxSize = .8f;
float minSize = .1f;

// Rotation
float rotate_speed = 0.5f;
float rotationA = 0.0f;

void CreateTriangle(VertexArray &va) {
	GLfloat vertices[] = {
		100.0f, 100.0f, 0.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 0.0f, 2.0f, 0.0f,
		200.0f, 200.0f, 0.0f, 2.0f, 2.0f,
		100.0f, 200.0f, 0.0f, 0.0f, 2.0f
	};

	unsigned int indices[] = {
		0, 1, 2,   // first triangle
		2, 3, 0    // second triangle
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vb = new VertexBuffer(vertices, 4 * 5 * sizeof(float));
	ib = new IndexBuffer(indices, 6);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(*vb, layout);
}

// Scale
float scaleA = 1.0f;
float max_scaleA = 3.0f;
float min_scaleA = 0.2f;
float scale_increment = 0.01f;

int main() {
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		glfwTerminate();
		return -1;
	}

	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backward Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "First OpenGL Project", NULL, NULL);

	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1);

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(window);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	VertexArray va;

	CreateTriangle(va);
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 1.0, 1.0f, 1.0f, 1.0f);

	Texture texture1("res/textures/brick.jpg");
	texture1.Bind();
	shader.SetUniform1i("texture1", 0);

	Texture texture2("res/textures/EddLogo.png");
	texture2.Bind();
	shader.SetUniform1i("texture2", 1);

	va.Unbind();
	shader.Unbind();
	vb->Unbind();
	ib->Unbind();

	Renderer renderer;
	InputManager input(window);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	
	glm::vec3 translationA(200, 200, 0);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		// Check for events
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer.Clear();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		float time = glfwGetTime(); // Lấy thời gian hiện tại
		float r = (sin(time * 0.5f) + 1.0f) * 0.5f;

		
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, translationA);
			model = glm::translate(model, glm::vec3(150.0f, 150.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotationA), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-150.0f, -150.0f, 0.0f));
			model = glm::scale(model, glm::vec3(scaleA, scaleA, 1.0f));
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniform4f("u_Color", r, .5f, .3f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp);

			// bind textures on corresponding texture units
			texture1.Bind(0);
			texture2.Bind(1);

			renderer.Draw(va, *ib, shader);
		}

		translationA.x += input.GetAxisX(.5f);
		translationA.y += input.GetAxisY(.5f);
		scaleA = input.AdjustScale(scaleA);
		rotationA = input.AdjustRocation(rotationA);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Inspector:");

			ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
			ImGui::SliderFloat("Scale A", &scaleA, min_scaleA, max_scaleA);
			ImGui::SliderFloat("Rotation A", &rotationA, -360.0f, 360.0f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}