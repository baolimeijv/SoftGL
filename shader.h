#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<fstream>
#include<sstream>
#include<iostream>

class shader
{
private:
	unsigned int shaderProgramID;
	std::string readFile(const char* path);
	unsigned int compileShader(unsigned int type, const char* source);
	void checkLinkFalse();
public:
	shader(const char* vertexPath, const char* fragmentPath);
	~shader();
	void use();
	void setInt(const std::string& name, int value);
	void setVec3(const std::string& name, float x, float y, float z);
	void setVec3(const std::string& name, const glm::vec3& value);
	void setMat4(const std::string& name, const glm::mat4& mat);
};