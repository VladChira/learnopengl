#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include "../TextureManager.hpp"
#include "../Shader.hpp"

class PhongMaterial : public Material
{
public:
    PhongMaterial()
        : Material(MaterialType::Phong), ka(0.1f),
          kd(1.0f), diffuseMap(nullptr), useDiffuseMap(false),
          ks(0.0f), specularMap(nullptr), useSpecularMap(false),
          shininess(10.0f)
    {
        ambient[0] = 0.4f;
        ambient[1] = 0.4f;
        ambient[2] = 0.4f;

        diffuse[0] = 1.0f;
        diffuse[1] = 1.0f;
        diffuse[2] = 1.0f;

        specular[0] = 0.5f;
        specular[1] = 0.5f;
        specular[2] = 0.5f;
    }

    PhongMaterial(std::string name)
        : PhongMaterial()
    {
        this->setName(name);
    }

    float ka;
    float ambient[3];

    float kd;
    float diffuse[3];
    std::shared_ptr<Texture> diffuseMap;
    bool useDiffuseMap;

    float ks;
    float specular[3];
    std::shared_ptr<Texture> specularMap;
    bool useSpecularMap;

    float shininess;

    void setUniforms(Shader &shader) override
    {
        shader.setFloat("material.ka", ka);
        shader.setVec3("material.ambient", ambient[0], ambient[1], ambient[2]);

        shader.setFloat("material.kd", kd);
        shader.setVec3("material.diffuse", diffuse[0], diffuse[1], diffuse[2]);
        shader.setBool("material.useDiffuseMap", useDiffuseMap);
        if (useDiffuseMap && diffuseMap != nullptr)
        {
            glActiveTexture(GL_TEXTURE0);
            shader.setInt("material.diffuseMap", 0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
        }

        shader.setFloat("material.ks", ks);
        shader.setVec3("material.specular", specular[0], specular[1], specular[2]);
        if (specularMap && specularMap != nullptr)
        {
            glActiveTexture(GL_TEXTURE1);
            shader.setInt("material.specularMap", 1);
            glBindTexture(GL_TEXTURE_2D, specularMap->id);
        }
        shader.setFloat("material.shininess", shininess);
    }
};