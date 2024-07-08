#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

enum class TextureType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

struct Texture
{
    unsigned int id;
    TextureType type;
    std::string path;
};

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    std::shared_ptr<Texture> loadTexture(std::string path, TextureType type);

    TextureManager(TextureManager &other) = delete;
    TextureManager(TextureManager &&) = delete;
    void operator=(const TextureManager &) = delete;

    static TextureManager *GetInstance();
    static void DestroyInstance();

    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

private:
    static TextureManager *tm_instance;

    unsigned int TextureFromFile(std::string filename, bool gamma)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load from: " << filename << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};