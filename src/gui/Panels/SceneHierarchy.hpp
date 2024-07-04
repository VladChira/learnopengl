#pragma once

#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../entities/Entity.hpp"
#include "../../SceneManager.hpp"

void layOutSceneHierarchy()
{
    for (int i = 0; i < SceneManager::GetInstance()->getEntityCount(); i++)
    {
        auto entity = SceneManager::GetInstance()->getEntityByIndex(i);
        if (ImGui::TreeNodeEx(entity->getName().c_str()))
        {
            ImGui::TreePop();
        }
    }
}
