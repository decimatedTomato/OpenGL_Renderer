#pragma once
#include <string>
#include <unordered_map>

#include "defines.hpp"

struct ShaderProgramSource
{
	const char* VertexShaderSource;
	const char* FragmentShaderSource;
};

class Shader
{
private:
	u32 rendererID_;
	ShaderProgramSource source_;
	std::string vertexShaderFilepath_;
	std::string fragmentShaderFilepath_;
	std::unordered_map<std::string, i32> uniformCache_;

public:
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1f(const std::string& name, f32 v);
private:
	u32 CompileShader(u32 type, const char* shaderSource) const;
	void CompileShaderProgram() const;
	i32 GetUniformLocation(const std::string& name);
};