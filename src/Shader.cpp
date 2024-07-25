#include <fstream>

#include "Shader.hpp"
#include "Renderer.hpp"

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath)
	:rendererID_(0)
	, vertexShaderFilepath_(vertexShaderFilepath)
	, fragmentShaderFilepath_(fragmentShaderFilepath)
{
	std::ifstream t1(vertexShaderFilepath);
	std::string s1((std::istreambuf_iterator<char>(t1)), std::istreambuf_iterator<char>());
	source_.VertexShaderSource = s1.c_str();
	std::ifstream t2(fragmentShaderFilepath);
	std::string s2((std::istreambuf_iterator<char>(t2)), std::istreambuf_iterator<char>());
	source_.FragmentShaderSource = s2.c_str();
	GLCall(rendererID_ = glCreateProgram());
	CompileShaderProgram();
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(rendererID_));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(rendererID_));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}


u32 Shader::CompileShader(u32 type, const char* shaderSource) const
{
	GLCall(u32 id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &shaderSource, nullptr));
	GLCall(glCompileShader(id));

	/* Error handling */
	i32 result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		i32 length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)malloc(sizeof(char) * length);
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") <<
			" shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return GL_FALSE;
	}
	return id;
}

void Shader::CompileShaderProgram() const
{
	u32 vs = CompileShader(GL_VERTEX_SHADER, source_.VertexShaderSource);
	u32 fs = CompileShader(GL_FRAGMENT_SHADER, source_.FragmentShaderSource);
	GLCall(glAttachShader(rendererID_, vs));
	GLCall(glAttachShader(rendererID_, fs));
	GLCall(glLinkProgram(rendererID_));
	GLCall(glValidateProgram(rendererID_));
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

i32 Shader::GetUniformLocation(const std::string& name)
{
	if (uniformCache_.find(name) != uniformCache_.end())
		//return uniformCache_[name];
		return uniformCache_[name];

	GLCall(i32 location = glGetUniformLocation(rendererID_, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
	}
	//uniformCache_[name] = location;
	uniformCache_[name] = location;
	return location;
}

void Shader::SetUniform1f(const std::string& name, f32 v)
{
	GLCall(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform2fv(const std::string& name, u32 count, Vec2f* v)
{
	GLCall(glUniform2fv(GetUniformLocation(name), count, (const GLfloat*)v));
}

void Shader::SetUniform3fv(const std::string& name, u32 count, Vec3f* v)
{
	GLCall(glUniform3fv(GetUniformLocation(name), count, (const GLfloat*)v));
}

void Shader::SetUniform2i(const std::string& name, Vec2i v)
{
	GLCall(glUniform2i(GetUniformLocation(name), v.x, v.y));
}

void Shader::SetUniform1i(const std::string& name, i32 v)
{
	GLCall(glUniform1i(GetUniformLocation(name), v));
}
