#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID_;
	unsigned int count_;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	// void LoadData(const void *data)

	inline unsigned int GetCount() const { return count_; }
};