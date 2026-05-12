#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Model.h"

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 相机参数
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 光源参数
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

// 鼠标参数
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 200.0f;
float lastY = 150.0f;
bool firstMouse = true;

// 移动参数
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cameraSpeed = 5.0f;
float lightSpeed = 0.1f;

float Angle = 0.0f;
float rotSpeed = 0.5f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float velocity = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraUp * velocity;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraUp * velocity;
    //移动光照
    float lightSpeed = 3.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos.x -= lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos.x += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos.z += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos.z -= lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lightPos.y += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightPos.y -= lightSpeed;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cube with Texture & Light", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    std::vector<float> vertices = {
        // 前面
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        // 后面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        // 上面
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        // 下面
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        // 右面
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         // 左面
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,   0, 2, 3,
        4, 6, 5,   4, 7, 6,
        8, 9, 10,  8, 10, 11,
        12,14,13,  12,15,14,
        16,17,18,  16,18,19,
        20,22,21,  20,23,22
    };

    Mesh mesh;
    mesh.BindBuffer(vertices, indices);

    std::vector<glm::mat4>model;
    for(int i=0;i<2;i++)
        model.push_back(glm::mat4(1.0f));
    model[1] = glm::translate(model[1], glm::vec3(3.0, 0,0));

    shader ourShader("vertexshadersource.glsl", "fragmentshadersource.glsl");

    Texture ourTex("TextureSrouce/RoadLines020B.png");
    Texture normalTex("TextureSrouce/RoadLines020B_1K-PNG_NormalGL.png");

    Model ourModel("bunny_10k.obj");

    //不能复用上面数据因为已经释放了，立方体模拟灯光位置
    std::vector<float> lightBoxVertices = {
        // 前面
        -0.1f, -0.1f,  0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.1f, -0.1f,  0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.1f,  0.1f,  0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.1f,  0.1f,  0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        // 后面
        -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
         0.1f, -0.1f, -0.1f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
         0.1f,  0.1f, -0.1f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f,
        -0.1f,  0.1f, -0.1f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,
        // 上面
        -0.1f,  0.1f,  0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.1f,  0.1f,  0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.1f,  0.1f, -0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.1f,  0.1f, -0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        // 下面
        -0.1f, -0.1f,  0.1f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f,
         0.1f, -0.1f,  0.1f,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f,
         0.1f, -0.1f, -0.1f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,
        -0.1f, -0.1f, -0.1f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,
        // 右面
         0.1f, -0.1f,  0.1f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.1f, -0.1f, -0.1f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.1f,  0.1f, -0.1f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.1f,  0.1f,  0.1f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         // 左面
         -0.1f, -0.1f,  0.1f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         -0.1f, -0.1f, -0.1f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         -0.1f,  0.1f, -0.1f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -0.1f,  0.1f,  0.1f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    };

    std::vector<unsigned int> lightBoxIndices = {
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        8,9,10, 8,10,11,
        12,14,13, 12,15,14,
        16,17,18, 16,18,19,
        20,22,21, 20,23,22
    };

    Mesh lightBoxMesh;
    lightBoxMesh.BindBuffer(lightBoxVertices, lightBoxIndices);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("normalMap", 1);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        Angle = rotSpeed * deltaTime;

        // 变换矩阵
        model[0] = glm::rotate(model[0], Angle, glm::vec3(0.5f, 1.0f, 0.0f));
        model[1] = glm::rotate(model[1], Angle, glm::vec3(0.5f, 1.0f, 0.0f));
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("model", model[0]);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", cameraPos);
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        ourShader.setBool("useTexture", true);
        ourTex.bind(0);
        normalTex.bind(1);
        mesh.draw();

        ourShader.setMat4("model",model[1]);
        ourShader.setBool("useTexture", false);
        ourModel.Draw(ourShader);   // 此处会自动设置 modelColor

        // 绘制光源指示器
        ourShader.setBool("useTexture", false);
        ourShader.setVec3("modelColor", glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.4f));
        ourShader.setMat4("model", lightModel);

        lightBoxMesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}