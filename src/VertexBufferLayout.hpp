#pragma once
#include <vector>
#include "Renderer.hpp"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return sizeof(GLfloat);
		case GL_UNSIGNED_INT: return sizeof(GLuint);
		case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		}
		ASSERT(false && "Unsupported type fo vertex buffer element");
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements_;
	unsigned int stride_;
public:
	VertexBufferLayout()
		: stride_(0) {}
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		VertexBufferElement element = { GL_FLOAT, count, GL_FALSE };
		elements_.push_back(element);
		stride_ += count * sizeof(GLfloat);

	}
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		VertexBufferElement element = { GL_UNSIGNED_INT, count, GL_TRUE };
		elements_.push_back(element);
		stride_ += count * sizeof(GLuint);

	}
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		VertexBufferElement element = { GL_UNSIGNED_BYTE, count, false };
		elements_.push_back(element);
		stride_ += count * sizeof(GLubyte);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return elements_; }
	inline unsigned int GetStride() const { return stride_; }
};