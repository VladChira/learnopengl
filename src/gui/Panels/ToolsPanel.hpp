#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void toolsPanel(unsigned int addLightTex)
{
    ImGui::ImageButton("Add Light Button", (ImTextureID)addLightTex, ImVec2(50,50));
}