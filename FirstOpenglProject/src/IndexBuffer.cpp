#include "IndexBuffer.h"
#include "GL/glew.h"
//#include "Renderer.h"

//IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
//	: m_Count(count)
//{
//	ASSERT(sizeof(unsigned int) == sizeof(GLuint));
//
//	GLCall(glGenBuffers(1, &m_RendererID));
//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
//	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
//}
//
//IndexBuffer::~IndexBuffer()
//{
//	GLCall(glDeleteBuffers(1, &m_RendererID));
//}
//
//void IndexBuffer::Bind() const
//{
//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
//}
//
//void IndexBuffer::Unbind() const
//{
//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}