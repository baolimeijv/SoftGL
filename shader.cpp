#include "shader.h"

std::string shader::readFile(const char* path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer<< file.rdbuf();
	return buffer.str();
}

unsigned int shader::compileShader(unsigned int type, const char* source)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infolog[512];
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		std::cout << "shader compoile flase: " <<infolog<< std::endl;
	}
	return shader;
}

void shader::checkLinkFalse()
{
	int success;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infolog[512];
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infolog);
		std::cout << "program link false: " << infolog << std::endl;
	}
}

shader::shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode =readFile(vertexPath);
	std::string fragmentCode = readFile(fragmentPath);

	unsigned int vertex_shader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	unsigned int fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertex_shader);
	glAttachShader(shaderProgramID, fragment_shader);
	glLinkProgram(shaderProgramID);
	checkLinkFalse();

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

}

shader::~shader()
{
	glDeleteProgram(shaderProgramID);
}

void shader::use()
{
	glUseProgram(shaderProgramID);
}

void shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void shader::setVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z);
}

void shader::setVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), value.x, value.y, value.z);
}

void shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()),1, GL_FALSE, glm::value_ptr(mat));
}
