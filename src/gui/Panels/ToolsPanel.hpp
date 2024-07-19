#pragma once

#include <iostream>

#include <glad/glad.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void toolsPanel(unsigned int addLightTex)
{
    // ImGui::ImageButton("Add Light Button", 0, ImVec2(50,50));
    ImGui::Button("Point Light", ImVec2(0, 40));
    ImGui::Button("Directional Light", ImVec2(0, 40));
    ImGui::Button("Spot Light", ImVec2(0, 40));
    ImGui::Separator();

    ImGui::Button("Cube", ImVec2(0, 40));
    ImGui::Button("Sphere", ImVec2(0, 40));

    ImGui::Separator();
    ImGui::Button("Material", ImVec2(0, 40));
}