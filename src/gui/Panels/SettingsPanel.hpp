#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../SceneManager.hpp"
#include "../../renderers/OpenGlRenderer.hpp"

void rasterizerSettings(OpenGlRenderer *renderer)
{
    if (ImGui::CollapsingHeader("Rasterizer Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Preview Mode"))
        {
            SceneManager::GetInstance()->bgColor[0] = 0.0f;
            SceneManager::GetInstance()->bgColor[1] = 0.0f;
            SceneManager::GetInstance()->bgColor[2] = 0.0f;
            renderer->drawGrid = false;
            renderer->drawLightGizmos = false;
            renderer->drawTransformGizmos = false;
            renderer->wireframeMode = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Viewport Mode"))
        {
            SceneManager::GetInstance()->bgColor[0] = 0.125f;
            SceneManager::GetInstance()->bgColor[1] = 0.125f;
            SceneManager::GetInstance()->bgColor[2] = 0.125f;
            renderer->drawGrid = true;
            renderer->drawLightGizmos = true;
            renderer->drawTransformGizmos = true;
            renderer->wireframeMode = false;
        }

        float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.50f;
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker3("Background Color", SceneManager::GetInstance()->bgColor);

        ImGui::Checkbox("Enable grid", &renderer->drawGrid);
        ImGui::Checkbox("Enable light gizmos", &renderer->drawLightGizmos);
        ImGui::Checkbox("Enable transform gizmos", &renderer->drawTransformGizmos);
        ImGui::Checkbox("Wireframe Mode", &renderer->wireframeMode);
    }
}