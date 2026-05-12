#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexcount = 0;
    glm::vec3 diffuseColor;   // ²ÄÖÊÂş·´ÉäÑÕÉ«
    void Location_Ptr();
public:
    Mesh();
    ~Mesh();
    void BindBuffer(std::vector<float>& vertex, std::vector<unsigned int>& index);
    void draw();
    void setDiffuseColor(const glm::vec3& color);
    glm::vec3 getDiffuseColor() const;
};