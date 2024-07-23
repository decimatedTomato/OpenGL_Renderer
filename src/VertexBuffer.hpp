#pragma once

class VertexBuffer
{
private:
	unsigned int rendererID_;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	// void LoadData(const void *data)
};