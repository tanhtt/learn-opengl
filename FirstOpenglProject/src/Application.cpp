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
//IndexBuffer* ib = nullptr;

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
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vb = new VertexBuffer(vertices, 36 * 5 * sizeof(float));
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

	glEnable(GL_DEPTH_TEST);

	VertexArray va;

	CreateTriangle(va);

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
	float mixVal = 1;

	float lastime = 0.0f;

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float cameraFov = 30.0f;
	float maxFov = 70.0f;
	float minFov = 10.0f;
	float zCame = -3.0f;

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


		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastime;
		lastime = currentTime;
		
		{
			glm::mat4 proj = glm::perspective(glm::radians(cameraFov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, zCame)); ;

			for (unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				if (i == 0) {
					model = glm::rotate(model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
				}
				else {
					model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				}
				glm::mat4 mvp = proj * view * model;

				shader.Bind();
				shader.SetUniform4f("u_Color", 1.0f, .5f, .3f, 1.0f);
				shader.SetUniformMat4f("u_MVP", mvp);
				shader.SetUniform1f("mixVal", mixVal);

				texture1.Bind(0);
				texture2.Bind(1);

				renderer.DrawArray(va, shader, 36, 0);
			}
		}

		//translationA.x += input.GetAxisX(.5f);
		//translationA.y += input.GetAxisY(.5f);
		//scaleA = input.AdjustScale(scaleA);
		//rotationA = input.AdjustRocation(rotationA);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Inspector:");

			//ImGui::SliderFloat3("FOV", &translationA.x, 0.0f, 960.0f);
			ImGui::SliderFloat("FOV", &cameraFov, minFov, maxFov);
			ImGui::SliderFloat("Z Cam", &zCame, -2.0f, -50.0f);

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