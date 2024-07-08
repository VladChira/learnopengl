#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "../entities/Entity.hpp"
#include "../cameras/Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "../entities/Primitive.hpp"

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

    std::vector<std::shared_ptr<Primitive>> primitives;

    std::shared_ptr<Entity> selectedEntity;

private:
    std::vector<std::shared_ptr<Entity>> entities;

    static SceneManager *sm_instance;
};