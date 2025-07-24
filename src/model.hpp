#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "std_image.h"

#include "shader.hpp"
#include "mesh.hpp"

class Model {
public:
    std::vector<Texture> textures_loaded;

    Model(std::string &path) {
      loadModel(path);
    }
    void draw(Shader &shader) {
      for (auto mesh : meshes) {
        mesh.draw(shader);
      }
    }

private:
    std::vector<Mesh> meshes;
    std::string dir;

    void loadModel(std::string &path) {
      Assimp::Importer importer;
      // TODO Use a smart pointer here
      const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        std::exit(EXIT_FAILURE);
      }
      std::cout << path << std::endl;
      dir = path.substr(0, path.find_last_of('/'));

      processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode *node, const aiScene *scene, unsigned int lvl = 0) {
      std::string indent = std::string(lvl+1, ' ');
      for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        std::cout << indent + "processing mesh " << i+1 << "/" << node->mNumMeshes << std::endl;
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, lvl+1));
      }
      for (unsigned int i = 0; i < node->mNumChildren; i++) {
        std::cout << indent + "processing child " << i+1 << "/" << node->mNumChildren << std::endl;
        processNode(node->mChildren[i], scene, lvl+1);
      }
    }
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, unsigned int lvl = 0) {
      std::string indent = std::string(lvl+1, ' ');
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::vector<Texture> textures;
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // std::cout << indent + "processing vertex " << i+1 << "/" << mesh->mNumVertices << std::endl;
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        if (mesh->mTextureCoords[0]) {
          glm::vec2 vec;
          vec.x = mesh->mTextureCoords[0][i].x;
          vec.y = mesh->mTextureCoords[0][i].y;
          vertex.texCoord = vec;
        } else {
          vertex.texCoord = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
      }
      for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        // std::cout << indent + "processing face " << i+1 << "/" << mesh->mNumFaces << std::endl;
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
          indices.push_back(face.mIndices[j]);
        }
      }
      if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTexture(*material, aiTextureType_DIFFUSE, "texture_diffuse", lvl);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTexture(*material, aiTextureType_SPECULAR, "texture_specular", lvl);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
      }
      return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> loadMaterialTexture(aiMaterial &mat,
        aiTextureType type, std::string typeName, unsigned int lvl)
    {
      std::string indent = std::string(lvl+1, ' ');
      std::vector<Texture> textures;
      for (unsigned int i = 0; i < mat.GetTextureCount(type); i++) {
        std::cout << indent + "loading material texture " << i+1 << "/" << mat.GetTextureCount(type) << std::endl;
        aiString str;
        mat.GetTexture(type, i, &str);
        bool skip = false;
        std::string path = std::string(str.C_Str());
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
          if (textures_loaded[j].path == path == 0) {
            textures.push_back(textures_loaded[j]);
            skip = true;
            break;
          }
        }
        if (skip) continue;
        Texture texture;
        texture.id = textureFromFile(path, dir);
        texture.type = typeName;
        texture.path = std::string(str.C_Str());
        textures_loaded.push_back(texture);
      }
      return textures;
    }

    unsigned int textureFromFile(std::string &path, std::string &dir) {
        std::string fname = dir + "/" + path;
        unsigned int id;
        glGenTextures(1, &id);
        int width, height, nrComponents;
        unsigned char *data = stbi_load(fname.c_str(), &width, &height, &nrComponents, 0);
        if (!data) {
            std::cout << "failed to load texture " << fname << std::endl;
            std::exit(EXIT_FAILURE);
        }
        GLenum format;
        if (nrComponents == 1)
          format = GL_RED;
        else if (nrComponents == 3)
          format = GL_RGB;
        else if (nrComponents == 4)
          format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        return id;
    };
};

#endif
