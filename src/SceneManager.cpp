#include "SceneManager.hpp"

SceneManager *SceneManager::sm_instance{nullptr};


std::shared_ptr<Entity> SceneManager::getEntityByID(std::string UUID)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->getUUID() == UUID)
            return entities[i];
    }
    return nullptr;
}

std::shared_ptr<Entity> SceneManager::getEntityByIndex(unsigned int index)
{
    if (index >= entities.size())
        return nullptr;
    return entities[index];
}

SceneManager::SceneManager()
{
    // A scene always has at least one camera
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    camera->setName("Default Camera");
    this->entities.push_back(camera);
    this->activeCamera = camera;

    // Set the clear color to black by default
    bgColor[0] = 0.0f;
    bgColor[1] = 0.0f;
    bgColor[2] = 0.0f;
    bgColor[3] = 1.0f;
}

SceneManager::~SceneManager()
{
    DestroyInstance();
}

SceneManager *SceneManager::GetInstance()
{
    if (sm_instance == nullptr)
        sm_instance = new SceneManager();
    return sm_instance;
}

void SceneManager::DestroyInstance()
{
    delete sm_instance;
    sm_instance = nullptr;
}