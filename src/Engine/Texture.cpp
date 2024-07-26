#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& textureFilepath)
	: rendererID_(0)
	, filePath_(textureFilepath)
	, localBuffer_(nullptr)
	, width_(0), height_(0), channels_(0)
	, textureMap_(0)
{
	stbi_set_flip_vertically_on_load(1);
	u8* localBuffer_ = stbi_load(textureFilepath.c_str(), &width_, &height_, &channels_, STBI_rgb_alpha);
	if (localBuffer_ == nullptr) {
		std::cout << "Error: Cannot load texture '" << textureFilepath << "'." << std::endl;
		ASSERT(false);
	}

	GLCall(glGenTextures(1, &textureMap_));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureMap_));

	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	// For non-power-of-two-dimensions
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer_));
}

Texture::~Texture()
{
	deleteLocalData();
	GLCall(glDeleteTextures(1, &textureMap_));
}

void Texture::Bind(u32 slot = 0) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureMap_));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

b32 Texture::deleteLocalData()
{
	if (localBuffer_ == nullptr) return false;
	stbi_image_free(localBuffer_);
	localBuffer_ = nullptr;
	return true;
}
