#pragma once

#include "Defines.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "IndexBuffer.hpp"

class Model
{
public:
private:
	u32 rendererID_;
	std::string filePath_;
	VertexArray va_;
	IndexBuffer ib_;
	i32 vertex_count_;
public:
	Model(const std::string& modelFilepath);
	~Model();

	VertexArray getVertexArray() const;
	i32 getVertexCount() const;
};