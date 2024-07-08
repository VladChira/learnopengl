#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include "../TextureManager.hpp"
#include "../Shader.hpp"

class PhongMaterial : public Material
{
public:
    PhongMaterial()
        : Material(MaterialType::Phong), ka(0.1f), ambient(0.1f),
          kd(1.0f), diffuse(1.0f), diffuseMap(nullptr), useDiffuseMap(false),
          ks(0.6f), specular(0.5f), specularMap(nullptr), useSpecularMap(false),
          shininess(10.0f)
    {
    }

    PhongMaterial(std::string name)
        : PhongMaterial()
    {
        this->setName(name);
    }

    float ka;
    glm::vec3 ambient;

    float kd;
    glm::vec3 diffuse;
    std::shared_ptr<Texture> diffuseMap;
    bool useDiffuseMap;

    float ks;
    glm::vec3 specular;
    std::shared_ptr<Texture> specularMap;
    bool useSpecularMap;

    float shininess;

    void setUniforms(Shader &shader)
    {
        shader.setFloat("material.ka", ka);
        shader.setVec3("material.ambient", ambient);

        shader.setFloat("material.kd", kd);
        shader.setVec3("material.diffuse", diffuse);
        shader.setBool("material.useDiffuseMap", useDiffuseMap);
        if (useDiffuseMap && diffuseMap != nullptr)
        {
            glActiveTexture(GL_TEXTURE0);
            shader.setInt("material.diffuseMap", 0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
        }

        shader.setFloat("material.ks", ks);
        shader.setVec3("material.specular", specular);
        if (specularMap && specularMap != nullptr)
        {
            glActiveTexture(GL_TEXTURE1);
            shader.setInt("material.specularMap", 1);
            glBindTexture(GL_TEXTURE_2D, specularMap->id);
        }
        shader.setFloat("material.shininess", shininess);
    }
};