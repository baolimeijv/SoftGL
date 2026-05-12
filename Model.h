#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>
#include <string>

class Mesh;   // 前向声明
class shader; // 前向声明

class Model {
public:
    Model(const char* path);
    void Draw(shader& shader);

private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
};