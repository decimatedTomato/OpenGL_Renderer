#pragma once
#include "defines.hpp"

class VertexBuffer
{
private:
	u32 rendererID_;
public:
	VertexBuffer(const void* data, u32 size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	// void LoadData(const void *data)
};