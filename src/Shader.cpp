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
	//glDeleteShader()
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


unsigned int Shader::CompileShader(unsigned int type, const char* shaderSource) const
{
	GLCall(unsigned int id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &shaderSource, nullptr));
	GLCall(glCompileShader(id));

	/* Error handling */
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
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
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, source_.VertexShaderSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source_.FragmentShaderSource);
	GLCall(glAttachShader(rendererID_, vs));
	GLCall(glAttachShader(rendererID_, fs));
	GLCall(glLinkProgram(rendererID_));
	GLCall(glValidateProgram(rendererID_));
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (uniformCache_.find(name) != uniformCache_.end())
		//return uniformCache_[name];
		return uniformCache_[name];

	GLCall(int location = glGetUniformLocation(rendererID_, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
	}
	//uniformCache_[name] = location;
	uniformCache_[name] = location;
	return location;
}

void Shader::SetUniform1f(const std::string& name, float v)
{
	GLCall(glUniform1f(GetUniformLocation(name), v));
}