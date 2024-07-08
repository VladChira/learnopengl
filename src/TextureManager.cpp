#include "TextureManager.hpp"

#include "gui/Console.hpp"

TextureManager *TextureManager::tm_instance{nullptr};

std::shared_ptr<Texture> TextureManager::loadTexture(std::string filename, TextureType type)
{
    // First, check if we already have this texture cached
    if (textures.find(filename) != textures.end())
        return textures[filename];

    // We don't have, load it now
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
    newTexture->id = TextureFromFile(filename, true);
    newTexture->type = type;
    newTexture->path = filename;
    
    textures.insert(std::make_pair(filename, newTexture));

    Console::GetInstance()->addSuccesEntry("Loaded texture from " + filename);

    return newTexture;
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    
}

TextureManager *TextureManager::GetInstance()
{
    if (tm_instance == nullptr)
        tm_instance = new TextureManager();
    return tm_instance;
}

void TextureManager::DestroyInstance()
{
    delete tm_instance;
    tm_instance = nullptr;
}
