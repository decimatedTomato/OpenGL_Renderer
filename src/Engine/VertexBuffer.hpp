#pragma once
#include "defines.hpp"

class VertexBuffer
{
private:
	u32 rendererID_;
public:
	VertexBuffer(const void* data, size size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	// void LoadData(const void *data)
};