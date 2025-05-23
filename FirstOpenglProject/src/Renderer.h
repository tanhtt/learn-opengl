#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:
	void Clear() const;
	void DrawElement(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawArray(const VertexArray& va, const Shader& shader, int count, int first = 0) const;
};