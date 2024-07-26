#pragma once
#include "defines.hpp"

class IndexBuffer
{
private:
	u32 rendererID_;
	u32 count_;
public:
	IndexBuffer(const u32* data, u32 count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	// void LoadData(const void *data)

	inline u32 GetCount() const { return count_; }
};