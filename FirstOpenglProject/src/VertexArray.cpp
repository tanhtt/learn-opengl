#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "iostream"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RenderId));
	std::cout << "VAO ID: " << m_RenderId << std::endl;
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RenderId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		std::cout << "VAO OK: " << m_RenderId << std::endl;
		std::cout << "Element " << i << ": count=" << element.count << ", type=" << element.type << ", stride=" << layout.GetStride()
			<< ", normalized=" << element.normalized << ", offset=" << offset << std::endl;
		GLCall(glVertexAttribPointer(i, element.count, element.type, GL_FALSE, layout.GetStride(), (const void*) offset));
		GLCall(glEnableVertexAttribArray(i));

		std::cout << "Stride float:" << (3 * sizeof(GLfloat)) << std::endl;
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RenderId));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
