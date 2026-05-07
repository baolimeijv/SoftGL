#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<vector>
class Mesh
{

private:
	unsigned int VAO, VBO, EBO;
	unsigned int indexcount=0;
	void Location_Ptr();
public:
	Mesh();
	~Mesh();
	void BindBuffer(std::vector<float>&vertex,std::vector<unsigned int>&index);
	void draw();
};