#include "stb_image.h"
#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static bool isGLB(const std::string& path) {
    auto ext = path.substr(path.find_last_of('.') + 1);
    return ext == "glb" || ext == "gltf";
}

Model::Model(const std::string& path) {
    directory = path.substr(0, path.find_last_of('/'));

    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals;
    if (!isGLB(path)) flags |= aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp erro: " << importer.GetErrorString() << "\n";
        return;
    }

    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* aimesh = scene->mMeshes[m];
        std::vector<float> verts;

        for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
            aiFace& face = aimesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                unsigned int idx = face.mIndices[j];

                verts.push_back(aimesh->mVertices[idx].x);
                verts.push_back(aimesh->mVertices[idx].y);
                verts.push_back(aimesh->mVertices[idx].z);

                if (aimesh->mTextureCoords[0]) {
                    verts.push_back(aimesh->mTextureCoords[0][idx].x);
                    verts.push_back(aimesh->mTextureCoords[0][idx].y);
                } else {
                    verts.push_back(0.0f);
                    verts.push_back(0.0f);
                }

                if (aimesh->HasNormals()) {
                    verts.push_back(aimesh->mNormals[idx].x);
                    verts.push_back(aimesh->mNormals[idx].y);
                    verts.push_back(aimesh->mNormals[idx].z);
                } else {
                    verts.push_back(0.0f);
                    verts.push_back(1.0f);
                    verts.push_back(0.0f);
                }
            }
        }

        GLuint tex = 0;
        if (aimesh->mMaterialIndex >= 0) {
            aiMaterial* mat = scene->mMaterials[aimesh->mMaterialIndex];
            aiTextureType types[] = { aiTextureType_BASE_COLOR, aiTextureType_DIFFUSE };
            for (aiTextureType type : types) {
                if (mat->GetTextureCount(type) > 0) {
                    aiString texPath;
                    mat->GetTexture(type, 0, &texPath);
                    std::string p = texPath.C_Str();
                    if (!p.empty() && p[0] == '*') {
                        int embIdx = std::stoi(p.substr(1));
                        tex = loadTextureEmbedded(scene->mTextures[embIdx]);
                    } else {
                        tex = loadTexture(directory + "/" + p);
                    }
                    break;
                }
            }
        }

        entries.push_back({ std::make_unique<Mesh>(verts), tex });
    }
}

void Model::draw(Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    shader.setMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));

    for (auto& entry : entries) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, entry.texture);
        shader.setBool("hasTexture", entry.texture != 0);
        entry.mesh->draw();
    }
}

GLuint Model::loadTexture(const std::string& path) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);
    if (data) {
        GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cerr << "Falha ao carregar textura: " << path << "\n";
    }
    return id;
}

GLuint Model::loadTextureEmbedded(const aiTexture* tex) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(false);
    int w, h, ch;

    if (tex->mHeight == 0) {
        // comprimida (PNG/JPG dentro do GLB)
        unsigned char* data = stbi_load_from_memory(
            reinterpret_cast<unsigned char*>(tex->pcData),
            tex->mWidth, &w, &h, &ch, 0);
        if (data) {
            GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else {
            std::cerr << "Falha ao decodificar textura embeddada\n";
        }
    } else {
        // raw ARGB8888 — Assimp armazena como BGRA, converte para RGBA
        w = tex->mWidth; h = tex->mHeight;
        std::vector<unsigned char> rgba(w * h * 4);
        for (int i = 0; i < w * h; i++) {
            rgba[i*4+0] = tex->pcData[i].r;
            rgba[i*4+1] = tex->pcData[i].g;
            rgba[i*4+2] = tex->pcData[i].b;
            rgba[i*4+3] = tex->pcData[i].a;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return id;
}
