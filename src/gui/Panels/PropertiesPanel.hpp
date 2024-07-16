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
#include "../../lights/PointLight.hpp"
#include "../../lights/DirectionalLight.hpp"

void displayBaseEntityProperties(std::shared_ptr<Entity> entity);
void displayPrimitiveEntityProperties(std::shared_ptr<Entity> ent);
void displayModelEntityProperties(std::shared_ptr<Entity> ent);
void displayLightEntityProperties(std::shared_ptr<Entity> ent);
void displayMaterialProperties(std::shared_ptr<Entity> ent);

static void HelpMarker(const char *desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void displayPropertiesOfEntity(std::shared_ptr<Entity> entity)
{
    if (entity == nullptr)
        return;

    displayBaseEntityProperties(entity);
    ImGui::Separator();

    switch (entity->getType())
    {
    case EntityType::Primitive:

        displayPrimitiveEntityProperties(entity);
        break;

    case EntityType::Model:
        displayModelEntityProperties(entity);
        break;

    case EntityType::Light:
        displayLightEntityProperties(entity);
        break;
    default:
        break;
    }
}

void displayMaterialProperties(std::shared_ptr<Entity> ent)
{
    if (ImGui::CollapsingHeader("Material Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Entity *entity = ent.get();
        Material *mat = dynamic_cast<Material *>(entity);

        ImGui::Separator();

        ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

        ImGui::Text("Material Type:");
        ImGui::SameLine();

        ImGui::ButtonEx(Material::MaterialTypeToString(mat->getMaterialType()).c_str(), sz);
        ImGui::PopItemFlag();

        if (mat->getMaterialType() == MaterialType::Phong)
        {
            PhongMaterial *p_mat = dynamic_cast<PhongMaterial *>(mat);
            if (p_mat == nullptr)
                return;

            ImGui::Checkbox("Use diffuse texture", &(p_mat->useDiffuseMap));
            ImGui::ColorEdit3("Diffuse Color", p_mat->diffuse);
            ImGui::SliderFloat("kD", &(p_mat->kd), 0.0f, 1.0f);
            std::string texturePath = "[no texture]";
            if (p_mat->diffuseMap != nullptr)
                texturePath = p_mat->diffuseMap->path;
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::Text("Diffuse texture path:");
            ImGui::SameLine();
            ImGui::ButtonEx(texturePath.c_str(), sz);
            ImGui::PopItemFlag();

            ImGui::Separator();

            ImGui::Checkbox("Use specular texture", &(p_mat->useSpecularMap));
            ImGui::ColorEdit3("Specular Color", p_mat->specular);
            ImGui::SliderFloat("kS", &(p_mat->ks), 0.0f, 1.0f);
            texturePath = "[no texture]";
            if (p_mat->specularMap != nullptr)
                texturePath = p_mat->specularMap->path;
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::Text("Specular texture path:");
            ImGui::SameLine();
            ImGui::ButtonEx(texturePath.c_str(), sz);
            ImGui::PopItemFlag();
        }
    }
}

void displayBaseEntityProperties(std::shared_ptr<Entity> entity)
{
    if (ImGui::CollapsingHeader("Entity Information", ImGuiTreeNodeFlags_DefaultOpen))
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

    if (entity->getType() == EntityType::Material)
    {
        displayMaterialProperties(entity);
        return;
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("Entity Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Reset Transform"))
        {
            entity->resetTransform();
        }
        // Create a table with 4 columns (one for labels and three for X, Y, Z)

        if (ImGui::BeginTable("TransformTable", 4, ImGuiTableFlags_Borders))
        {
            ImU32 red_bg_color = ImGui::GetColorU32(ImVec4(0.8f, 0.02f, 0.01f, 1.0f));
            ImU32 green_bg_color = ImGui::GetColorU32(ImVec4(0.16f, 0.83f, 0.02f, 1.0f));
            ImU32 blue_bg_color = ImGui::GetColorU32(ImVec4(0.01f, 0.29f, 0.878f, 1.0f));

            float position[3], rotation[3], scale[3];
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(entity->transform), position, rotation, scale);

            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, 14288647);
            ImGui::TableNextColumn();
            ImGui::Text("Position");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, red_bg_color);
            ImGui::DragFloat("##posX", &position[0], 0.01f, 0.0f, 0.0f, "X: %.2f");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, green_bg_color);
            ImGui::DragFloat("##posY", &position[1], 0.01f, 0.0f, 0.0f, "Y: %.2f");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, blue_bg_color);
            ImGui::DragFloat("##posZ", &position[2], 0.01f, 0.0f, 0.0f, "Z: %.2f");

            ImGui::TableNextColumn();
            ImGui::Text("Rotation");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, red_bg_color);
            ImGui::DragFloat("##rotX", &rotation[0], 0.1f, 0.0f, 0.0f, "X: %.2f");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, green_bg_color);
            ImGui::DragFloat("##rotY", &rotation[1], 0.1f, 0.0f, 0.0f, "Y: %.2f");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, blue_bg_color);
            ImGui::DragFloat("##rotZ", &rotation[2], 0.1f, 0.0f, 0.0f, "Z: %.2f");

            ImGui::TableNextColumn();
            ImGui::Text("Scale");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, red_bg_color);
            ImGui::DragFloat("##scaleX", &scale[0], 0.01f, 0.0f, 0.0f, "X: %.2f");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, green_bg_color);
            ImGui::DragFloat("##scaleY", &scale[1], 0.01f, 0.0f, 0.0f, "Y: %.2f");
            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, blue_bg_color);
            ImGui::DragFloat("##scaleZ", &scale[2], 0.01f, 0.0f, 0.0f, "Z: %.2f");

            ImGui::EndTable();

            ImGuizmo::RecomposeMatrixFromComponents(position, rotation, scale, glm::value_ptr(entity->getTransform()));
        }
    }
}

void displayModelEntityProperties(std::shared_ptr<Entity> ent)
{
    if (ent->getType() != EntityType::Model)
        return;

    Entity *entity = ent.get();
    Model *model = dynamic_cast<Model *>(entity);
}

void displayPrimitiveEntityProperties(std::shared_ptr<Entity> ent)
{
    if (ent->getType() != EntityType::Primitive)
        return;

    Entity *entity = ent.get();
    Primitive *primitive = dynamic_cast<Primitive *>(entity);
}

void displayLightEntityProperties(std::shared_ptr<Entity> ent)
{
    if (ent->getType() != EntityType::Light)
        return;

    Entity *entity = ent.get();
    Light *light = dynamic_cast<Light *>(entity);

    if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (light->getType() == LightType::PointLight)
        {
            PointLight *pLight = dynamic_cast<PointLight *>(light);
            if (pLight == nullptr)
                return;
            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.50f;
            ImGui::SetNextItemWidth(w);
            ImGui::ColorPicker3("Light Color", pLight->lightColor);
            ImGui::DragFloat("Light Intensity", &(pLight->intensity), 0.05f, 0.0f, 10.0f);
            ImGui::Separator();
            ImGui::Text("Light Fallof Settings");
            ImGui::SameLine();
            HelpMarker("Change how the light gets dimmer with distance. Ctrl+Click to manually edit values");
            ImGui::SliderFloat("Constant Term", &(pLight->constantTerm), 0.5f, 1.5f);
            ImGui::SliderFloat("Linear Term", &(pLight->linearTerm), 0.01f, 0.1f);
            ImGui::SliderFloat("Quadratic Term", &(pLight->quadraticTerm), 0.01f, 0.08f);
        }

        if (light->getType() == LightType::DirectionalLight)
        {
            DirectionalLight *pLight = dynamic_cast<DirectionalLight *>(light);
            if (pLight == nullptr)
                return;

            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.50f;
            ImGui::SetNextItemWidth(w);
            ImGui::ColorPicker3("Light Color", pLight->lightColor);
            ImGui::SliderFloat("Light Intensity", &(pLight->intensity), 0.0f, 5.0f);
            ImGui::Separator();
            ImGui::DragFloat3("Light Direction", pLight->direction, .01f, -1.0f, 1.0f);
        }
    }
}