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
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"


const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;
VertexBuffer* vb = nullptr;
IndexBuffer* ib = nullptr;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDir = true;
float curSize = 0.4f;
float maxSize = .8f;
float minSize = .1f;

void CreateTriangle(VertexArray &va) {
	GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	vb = new VertexBuffer(vertices, 12 * sizeof(float));
	ib = new IndexBuffer(indices, 6);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(*vb, layout);
}

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

	// Set viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	VertexArray va;

	CreateTriangle(va);
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.3f, 0.3f, 0.5f, 1.0f);

	va.Unbind();
	shader.Unbind();
	vb->Unbind();
	ib->Unbind();

	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		// Check for events
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();

		float time = glfwGetTime(); // Lấy thời gian hiện tại
		float r = (sin(time * 0.5f) + 1.0f) * 0.5f;

		shader.SetUniform4f("u_Color", r, .3f, .5f, 1.0f);

		va.Bind();
		ib->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		glfwSwapBuffers(window);
	}
	return 0;
}