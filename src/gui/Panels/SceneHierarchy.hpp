#pragma once

#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../entities/Entity.hpp"
#include "../../SceneManager.hpp"

void displayNode(std::shared_ptr<Entity> entity)
{
    int noChildren = entity->getChildren().size();
    ImGuiTreeNodeFlags flags = 0;
    if (noChildren == 0)
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (ImGui::TreeNodeEx(entity->getName().c_str(), flags))
    {
        for (int i = 0; i < noChildren; i++)
        {
            displayNode(entity->getChildren()[i]);
        }

        ImGui::TreePop();
    }
}

void layOutSceneHierarchy()
{
    for (int i = 0; i < SceneManager::GetInstance()->getEntityCount(); i++)
    {
        displayNode(SceneManager::GetInstance()->getEntityByIndex(i));
    }
}
