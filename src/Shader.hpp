#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	const char* VertexShaderSource;
	const char* FragmentShaderSource;
};

class Shader
{
private:
	unsigned int rendererID_;
	ShaderProgramSource source_;
	std::string vertexShaderFilepath_;
	std::string fragmentShaderFilepath_;
	std::unordered_map<std::string, int> uniformCache_;

public:
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1f(const std::string& name, float v);
private:
	unsigned int CompileShader(unsigned int type, const char* shaderSource) const;
	void CompileShaderProgram() const;
	int GetUniformLocation(const std::string& name);
};