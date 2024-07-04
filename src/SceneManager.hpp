#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "../entities/Entity.hpp"
#include "../cameras/Camera.hpp"

class SceneManager
{

public:
    SceneManager();
    ~SceneManager();

    std::shared_ptr<Entity> getEntityByID(std::string UUID);
    std::shared_ptr<Entity> getEntityByIndex(unsigned int index);
    unsigned int getEntityCount() { return entities.size(); }

    static SceneManager *GetInstance();
    static void DestroyInstance();

    SceneManager(SceneManager &other) = delete;
    void operator=(const SceneManager &) = delete;
    SceneManager(SceneManager &&) = delete;

    float bgColor[4];
    std::shared_ptr<Camera> activeCamera;

private:
    std::vector<std::shared_ptr<Entity>> entities;

    static SceneManager *sm_instance;
};