#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>
#include <string>

class Mesh;   // 前向声明，避免循环包含

class Model {
public:
    Model(const char* path);
    void Draw();

private:
    // 改用智能指针持有 Mesh，彻底避免拷贝和移动语义的问题
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
};