#pragma once

#include "Defines.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "IndexBuffer.hpp"

class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear();
	void ClearColor(Vec4f color);
};