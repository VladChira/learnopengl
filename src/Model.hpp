#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "TextureManager.hpp"
#include "entities/Entity.hpp"

class Model : public Entity
{
public:
    // model data
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string directory;
    bool gammaCorrection;

    Model() : Entity(EntityType::Model) {}

    ~Model()
    {
    }

    void Init(std::string const &path);

    // draws the model, and thus all its meshes
    void Draw(Shader &shader);

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(Entity *entityNode, aiNode *node, glm::mat4 mat, const aiScene *scene, std::vector<std::shared_ptr<PhongMaterial>> materials);

    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, glm::mat4 mat, const aiScene *scene, std::vector<std::shared_ptr<PhongMaterial>> materials);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType ttype);
};