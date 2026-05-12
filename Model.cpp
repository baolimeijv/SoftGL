#include "Model.h"
#include "Mesh.h"
#include "shader.h"
#include <iostream>

Model::Model(const char* path)
{
    loadModel(path);
}

void Model::loadModel(const std::string& path) 
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) 
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) 
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        // 位置
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // 法线
        if (mesh->HasNormals())
        {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else 
        {
            vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
        }
        // 纹理坐标
        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else 
        {
            vertices.push_back(0.0f); vertices.push_back(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) 
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 读取材质漫反射颜色
    glm::vec3 matDiffuse(1.0f, 1.0f, 1.0f);
    if (mesh->mMaterialIndex >= 0) 
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            matDiffuse = glm::vec3(color.r, color.g, color.b);
        }
    }

    auto m = std::make_unique<Mesh>();
    m->BindBuffer(vertices, indices);
    m->setDiffuseColor(matDiffuse);
    meshes.push_back(std::move(m));
}

void Model::Draw(shader& shader) 
{
    if (meshes.empty()) return;
    glm::vec3 color = meshes[0]->getDiffuseColor();
    shader.setVec3("modelColor", color);
    for (auto& m : meshes) 
    {
        m->draw();
    }
}