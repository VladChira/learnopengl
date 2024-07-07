#include "SceneManager.hpp"

SceneManager *SceneManager::sm_instance{nullptr};

void SceneManager::addMesh(std::shared_ptr<Mesh> newMesh)
{
    // First, check if we don't already have this mesh included
    for (int i = 0; i < meshes.size(); i++)
        if (meshes[i]->getUUID() == newMesh->getUUID())
            return;

    meshes.push_back(newMesh);
    entities.push_back(newMesh);
}

void SceneManager::addModel(std::shared_ptr<Model> newModel)
{
    // First, check if we don't already have this model included
    for (int i = 0; i < models.size(); i++)
        if (models[i]->getUUID() == newModel->getUUID())
            return;

    models.push_back(newModel);
    entities.push_back(newModel);
}

void SceneManager::addCamera(std::shared_ptr<Camera> newCamera)
{
    // First, check if we don't already have this camera included
    for (int i = 0; i < cameras.size(); i++)
        if (cameras[i]->getUUID() == newCamera->getUUID())
            return;

    cameras.push_back(newCamera);
    entities.push_back(newCamera);
}

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
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(5.61f, 1.55f, 5.04), glm::vec3(0.0f, 1.0f, 0.0f), -136.9f, -12.6f);
    camera->setName("Default Camera");
    addCamera(camera);
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