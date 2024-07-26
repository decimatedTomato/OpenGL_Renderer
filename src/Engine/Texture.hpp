#pragma once

#include "Renderer.hpp"

class Texture
{
private:
	u32 rendererID_;
	std::string filePath_;
	u8* localBuffer_;
	i32 width_, height_, channels_;
	u32 textureMap_;
public:
	Texture(const std::string& textureFilepath);
	~Texture();

	void Bind(u32 slot) const;
	void Unbind() const;

	b32 deleteLocalData();
};