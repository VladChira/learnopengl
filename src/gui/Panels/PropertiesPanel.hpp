#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../entities/Entity.hpp"
#include "../../SceneManager.hpp"

void displayBaseEntityProperties(std::shared_ptr<Entity> entity);

void displayPropertiesOfEntity(std::shared_ptr<Entity> entity)
{
    if (entity == nullptr)
        return;
    displayBaseEntityProperties(entity);
}

void displayBaseEntityProperties(std::shared_ptr<Entity> entity)
{
    if (ImGui::CollapsingHeader("Base Entity", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

        ImGui::Text("Entity UUID:");
        ImGui::SameLine();
        
        ImGui::ButtonEx(entity->getUUID().c_str(), sz);

        ImGui::Text("Entity Name:");
        ImGui::SameLine();
        ImGui::Button(entity->getName().c_str(), sz);

        ImGui::Text("Entity Type:");
        ImGui::SameLine();
        ImGui::Button(Entity::EntityTypeToString(entity->getType()).c_str(), sz);

        ImGui::PopItemFlag();
    }
}