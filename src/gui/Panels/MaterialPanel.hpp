#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../SceneManager.hpp"

void layoutMaterials()
{
    float w = ImGui::GetContentRegionAvail().x;
    float padding = 16.0f;
    float previewSize = 150.0f;
    float cellSize = padding + previewSize;

    int numMatPerColumn = (int)(w / cellSize);
    if (numMatPerColumn < 1) numMatPerColumn = 1;

    ImGui::Columns(numMatPerColumn, 0, false);
    for (int i = 0; i < SceneManager::GetInstance()->materials.size(); i++)
    {
        auto mat = SceneManager::GetInstance()->materials[i];
        if (ImGui::ButtonEx((mat->getName() + "##" + mat->getUUID()).c_str(), ImVec2(previewSize, previewSize)))
        {
            ImGui::SetWindowFocus("Properties");
            SceneManager::GetInstance()->selectedEntity = mat;
        }
        ImGui::Text(mat->getName().c_str());

        ImGui::NextColumn();
    }
    ImGui::Columns(1);
}