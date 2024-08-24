#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const u32* data, u32 count)
	: count_(count)
{
	GLCall(glGenBuffers(1, &rendererID_));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererID_));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
