#pragma once

class VertexBuffer
{
private:
	unsigned int rendererID_;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();

	// void LoadData(const void *data)
};