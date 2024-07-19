#pragma once

#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "entities/Entity.hpp"
#include "SceneManager.hpp"

void displayNode(std::shared_ptr<Entity> currentEntity, std::shared_ptr<Entity> selectedEntity)
{
    int noChildren = currentEntity->getChildren().size();
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (noChildren == 0)
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (selectedEntity != nullptr && currentEntity->getUUID() == selectedEntity->getUUID())
        flags |= ImGuiTreeNodeFlags_Selected;

    bool node_open = ImGui::TreeNodeEx(currentEntity->getName().c_str(), flags);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        // Mark the current entity as the selected one and open the Properties panel
        SceneManager::GetInstance()->selectedEntity = currentEntity;
        ImGui::SetWindowFocus("Properties");
    }

    if (node_open)
    {
        for (int i = 0; i < noChildren; i++)
        {
            displayNode(currentEntity->getChildren()[i], selectedEntity);
        }
        ImGui::TreePop();
    }
}

void layOutSceneHierarchy()
{
    // FIXME: I don't like how the selected entity becomes null first and then is assigned
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(0)) SceneManager::GetInstance()->selectedEntity = nullptr;
    for (int i = 0; i < SceneManager::GetInstance()->getEntityCount(); i++)
    {
        auto selectedEntity = SceneManager::GetInstance()->selectedEntity;
        displayNode(SceneManager::GetInstance()->getEntityByIndex(i), selectedEntity);
    }
}
