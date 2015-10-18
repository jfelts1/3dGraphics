#include "ShaderUtils.h"

void Utils::CompileShader(GLuint shader)
{
	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		auto log = new GLchar[static_cast<size_t>(len + 1)];
		glGetShaderInfoLog(shader, len, &len, log);
		std::cerr << "Shader compilation failed: " << log << std::endl;
		delete[] log;
	}
}

void Utils::LinkShader(GLuint shader)
{
	glLinkProgram(shader);

	GLint linked;
	glGetProgramiv(shader, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &len);

		auto log = new GLchar[static_cast<size_t>(len + 1)];
		glGetProgramInfoLog(shader, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
	}
}

GLuint Utils::initShaders(const char *v_shader, const char *f_shader)
{
	auto program = glCreateProgram();
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	auto vertexShaderText = Utils::readFileAsText(v_shader);
	auto fragmentShaderText = Utils::readFileAsText(f_shader);

	auto tmp = const_cast<const GLchar*>(vertexShaderText.data());
	glShaderSource(vertexShader, 1, &tmp, nullptr);
	tmp = const_cast<const GLchar*>(fragmentShaderText.data());
	glShaderSource(fragmentShader, 1, &tmp, nullptr);

	CompileShader(vertexShader);
	CompileShader(fragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	LinkShader(program);

	glUseProgram(program);
	return program;
}