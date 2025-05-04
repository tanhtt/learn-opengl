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


const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;
VertexBuffer* vb = nullptr;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDir = true;
float curSize = 0.4f;
float maxSize = .8f;
float minSize = .1f;

struct ShaderProgrammSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgrammSource ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str()};
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);
	if (!theShader) {
		printf("Error creating shader type %d\n", shaderType);
		return;
	}

	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShader() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Error creating shader program\n");
		return;
	}

	ShaderProgrammSource source = ParseShader("res/shaders/Basic.shader");
	//std::cout << "Vertex Shader:\n" << source.VertexSource << std::endl;
	//std::cout << "Fragment Shader:\n" << source.FragmentSource << std::endl;

	AddShader(shader, source.VertexSource.c_str(), GL_VERTEX_SHADER);
	AddShader(shader, source.FragmentSource.c_str(), GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	//uniformModel = glGetUniformLocation(shader, "model");

}

void CreateTriangle(VertexArray &va) {
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//VertexBuffer vb(vertices, 9 * sizeof(float));
	vb = new VertexBuffer(vertices, 9 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(*vb, layout);

	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//std::cout << "VAO: " << VAO << std::endl;
	//
	//VertexBuffer vb(vertices, 6 * sizeof(float));
	//
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(0);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
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
	CompileShader();

	float r = 0.0f;
	float increment = 0.5f;

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		// Check for events
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glUseProgram(shader));

		//glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));


		//glUniform1f(uniformModel, triOffset);
		//GLCall(glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)));

		int location = glGetUniformLocation(shader, "u_Color");
		GLCall(glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f));

		if (r < 0.0f) {
			increment = 0.5f;
		}
		else if (r > 1.0f) {
			increment = -0.5f;
		}

		r += increment;

		va.Bind();
		//glBindVertexArray(VAO);

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

		GLCall(glUseProgram(0));

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	return 0;
}