#include "VertexBuffer.h"
#include "GL/glew.h"
//#include "Renderer.h"
//
//VertexBuffer::VertexBuffer(const void* data, unsigned int size)
//{
//	GLCall(glGenBuffers(1, &m_RendererID));
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
//	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
//}
//
//VertexBuffer::~VertexBuffer()
//{
//	GLCall(glDeleteBuffers(1, &m_RendererID));
//}
//
//void VertexBuffer::Bind() const
//{
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
//}
//
//void VertexBuffer::Unbind() const
//{
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
