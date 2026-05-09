#include "Model.h"
#include "Mesh.h"
#include <iostream>

Model::Model(const char* path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    //强制转为triangle 反转y轴  添加法向量
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    //文件读入不成功  按位与文件是否完整  根为空
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // 位置
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // 法线
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else {
            vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
        }
        // 纹理坐标
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0.0f); vertices.push_back(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    auto m = std::make_unique<Mesh>();
    m->BindBuffer(vertices, indices);
    meshes.push_back(std::move(m));
}

void Model::Draw() {
    for (auto& m : meshes) {
        m->draw();
    }
}