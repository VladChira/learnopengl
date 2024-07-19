#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "entities/Entity.hpp"
#include "cameras/Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "entities/Primitive.hpp"
#include "lights/Light.hpp"
#include "materials/PhongMaterial.hpp"

class SceneManager
{

public:
    SceneManager();
    ~SceneManager();

    std::shared_ptr<Entity> getEntityByID(std::string UUID);
    std::shared_ptr<Entity> getEntityByIndex(unsigned int index);
    unsigned int getEntityCount() { return entities.size(); }

    void addMesh(std::shared_ptr<Mesh> newMesh);
    void addModel(std::shared_ptr<Model> newModel);
    void addCamera(std::shared_ptr<Camera> newCamera);
    void addPrimitive(std::shared_ptr<Primitive> newPrimitive);
    void addLight(std::shared_ptr<Light> newLight);
    void addMaterial(std::shared_ptr<Material> newMaterial);

    static SceneManager *GetInstance();
    static void DestroyInstance();

    SceneManager(SceneManager &other) = delete;
    void operator=(const SceneManager &) = delete;
    SceneManager(SceneManager &&) = delete;

    float bgColor[4];
    std::shared_ptr<Camera> activeCamera;

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Model>> models;
    std::vector<std::shared_ptr<Camera>> cameras;

    std::vector<std::shared_ptr<Material>> materials;

    std::vector<std::shared_ptr<Light>> lights;
    int nrOfPointLights = 0;
    int nrOfDirLights = 0;
    int nrOfSpotLights = 0;

    std::vector<std::shared_ptr<Primitive>> primitives;

    std::shared_ptr<Entity> selectedEntity;

private:
    std::vector<std::shared_ptr<Entity>> entities;

    static SceneManager *sm_instance;
};