#pragma once
#include <vector>
#include "Renderer.hpp"

struct VertexBufferElement
{
	u32 type;
	u32 count;
	unsigned char normalized;

	static u32 GetSizeOfType(u32 type)
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
	u32 stride_;
public:
	VertexBufferLayout()
		: stride_(0) {}
	~VertexBufferLayout();

	template<typename T>
	void Push(u32 count)
	{
		static_assert(false);
	}

	template<>
	void Push<f32>(u32 count)
	{
		VertexBufferElement element = { GL_FLOAT, count, GL_FALSE };
		elements_.push_back(element);
		stride_ += count * sizeof(GLfloat);

	}
	template<>
	void Push<u32>(u32 count)
	{
		VertexBufferElement element = { GL_UNSIGNED_INT, count, GL_TRUE };
		elements_.push_back(element);
		stride_ += count * sizeof(GLuint);

	}
	template<>
	void Push<u8>(u32 count)
	{
		VertexBufferElement element = { GL_UNSIGNED_BYTE, count, false };
		elements_.push_back(element);
		stride_ += count * sizeof(GLubyte);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return elements_; }
	inline u32 GetStride() const { return stride_; }
};