#include "SceneManager.hpp"

#include "lights/PointLight.hpp"
#include "entities/CubePrimitive.hpp"
#include "lights/DirectionalLight.hpp"

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

void SceneManager::addPrimitive(std::shared_ptr<Primitive> newPrimitive)
{
    // First, check if we don't already have this primitive included
    for (int i = 0; i < primitives.size(); i++)
        if (primitives[i]->getUUID() == newPrimitive->getUUID())
            return;

    primitives.push_back(newPrimitive);
    entities.push_back(newPrimitive);
}

void SceneManager::addLight(std::shared_ptr<Light> newLight)
{
    // First, check if we don't already have this light included
    for (int i = 0; i < lights.size(); i++)
        if (lights[i]->getUUID() == newLight->getUUID())
            return;

    lights.push_back(newLight);
    entities.push_back(newLight);

    switch (newLight->getType())
    {
    case LightType::PointLight:
        nrOfPointLights++;
        break;
    case LightType::DirectionalLight:
        nrOfDirLights++;
        break;
    case LightType::SpotLight:
        nrOfSpotLights++;
        break;
    default:
        break;
    }
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
    camera->setName("Camera");
    addCamera(camera);
    this->activeCamera = camera;

    // Add a default light
    std::shared_ptr<PointLight> defaultLight = std::make_shared<PointLight>();
    defaultLight->setName("Point Light");
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.21f, 0.95f, 3.0f));
    defaultLight->transform = transform;
    addLight(defaultLight);

    std::shared_ptr<PointLight> defaultLight2 = std::make_shared<PointLight>();
    defaultLight2->setName("Point Light 2");
    defaultLight2->transform = transform;
    addLight(defaultLight2);

    // std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();
    // light->setName("Directional Light");
    // light->position[0] = -0.21f;
    // light->position[1] = 0.95f;
    // light->position[2] = 3.0f;
    // addLight(light);

    // And a default cube
    std::shared_ptr<CubePrimitive> defaultCube = std::make_shared<CubePrimitive>(1.0f, 1.0f, 1.0f);
    defaultCube->setName("Cube");
    addPrimitive(defaultCube);

    // Set the clear color to black by default
    bgColor[0] = 0.125f;
    bgColor[1] = 0.125f;
    bgColor[2] = 0.125f;
    bgColor[3] = 1.0f;

    // Nothing is selected in the hierarchy at first
    selectedEntity = nullptr;
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