#pragma once
#include <string>
#include <unordered_map>

#include "Defines.hpp"

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

	void SetUniform1f(const std::string& name, const f32 v);
	void SetUniform2f(const std::string& name, const Vec2f v);
	void SetUniform2fv(const std::string& name, u32 count, const f32* v);
	void SetUniform3fv(const std::string& name, u32 count, const f32* v);
	void SetUniform1i(const std::string& name, const i32 v);
	void SetUniform2i(const std::string& name, const Vec2i v);
	void SetUniformMat4x4f(const std::string& name, const f32* v);
	void SetUniformMat4fv(const std::string& name, u32 count, const f32* v);
private:
	u32 CompileShader(u32 type, const char* shaderSource) const;
	void CompileShaderProgram() const;
	i32 GetUniformLocation(const std::string& name);
};