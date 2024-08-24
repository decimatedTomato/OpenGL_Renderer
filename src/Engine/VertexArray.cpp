#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &rendererID_));
}
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &rendererID_));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	u32 offset = 0;
	for (u32 i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	vb.Unbind();
	Unbind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(rendererID_));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
