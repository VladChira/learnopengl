#include "Window.hpp"

#include <iostream>

#include "Console.hpp"
#include "Panels/SceneHierarchy.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Panels/SettingsPanel.hpp"
#include "Panels/ToolsPanel.hpp"
#include "Panels/MaterialPanel.hpp"

#include "stb_image.h"

#include "renderers/MaterialPreview.hpp"

const char *glsl_version = "#version 130";

void ConsoleLogWindow();

void embraceTheDarkness();

Window::Window(std::string title, const unsigned int width, const unsigned int height, bool enableVsync)
{
    this->title = title;
    this->windowData.width = width;
    this->windowData.height = height;
    this->initialized = false;
    this->vsyncEnabled = enableVsync;

    LOG_INFO("Initializing window...");
    this->Init();

    LOG_INFO("Initializing ImGui...");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->Fonts->AddFontFromFileTTF("../fonts/DMSans-Regular.ttf", 18.0);

    // ImGui::StyleColorsDark();
    embraceTheDarkness();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

Window::~Window()
{
    LOG_INFO("Closing window...");
    Shutdown();
}

void Window::onUpdate()
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    io->DisplaySize.x = (float)this->getWidth();
    io->DisplaySize.y = (float)this->getHeight();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuizmo::BeginFrame();

    // Draw the GUI here
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

    // Set up the main dock node
    static bool first_time = true;
    if (first_time)
    {
        first_time = false;

        // Get the central node
        ImGuiID dockspace_id = ImGui::GetMainViewport()->ID;
        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        // Split the dockspace into 3 sections
        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.035f, nullptr, &dock_main_id);
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.3f, nullptr, &dock_main_id);
        ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.3f, nullptr, &dock_main_id);

        // Split the left dock for Scene Hierarchy and Properties
        // ImGuiID dock_left_top_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Up, 0.5f, nullptr, &dock_left_id);
        // ImGuiID dock_left_bottom_id = dock_left_id;

        // Split the right dock for Stats and Settings
        ImGuiID dock_right_top_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Up, 0.5f, nullptr, &dock_right_id);
        ImGuiID dock_right_bottom_id = dock_right_id;

        // Dock windows
        ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_right_top_id);
        ImGui::DockBuilderDockWindow("Properties", dock_right_bottom_id);
        ImGui::DockBuilderDockWindow("Materials", dock_bottom_id);
        ImGui::DockBuilderDockWindow("Content Browser", dock_right_top_id);
        ImGui::DockBuilderDockWindow("Console", dock_bottom_id);
        ImGui::DockBuilderDockWindow("Settings", dock_right_bottom_id);
        ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

        ImGui::DockBuilderDockWindow("Tools", dock_left_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGuiWindowFlags menu_bar_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    if (ImGui::Begin("Viewport"))
    {
        if (renderer == nullptr)
            LOG_CRITICAL("Fatal error. No renderer has been added for preview");

        ImVec2 avail = ImGui::GetContentRegionAvail();

        // Define the aspect ratio of your scene
        float aspect_ratio = 16.0f / 9.0f;

        // Calculate the new dimensions preserving the aspect ratio
        float new_width = avail.x;
        float new_height = avail.x / aspect_ratio;

        if (new_height > avail.y)
        {
            new_height = avail.y;
            new_width = avail.y * aspect_ratio;
        }
        // Notify the renderer that the size of the viewport has changed
        renderer->OnResize(new_width, new_height);

        // Draw the texture with the calculated size
        unsigned int tex = renderer->getFrameBufferTexture();
        ImGui::Image(
            (ImTextureID)tex,
            ImVec2(new_width, new_height),
            ImVec2(0, 1),
            ImVec2(1, 0));

        bool gizmoSelected = false;
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::AllowAxisFlip(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, new_width, new_height);
        auto entity = SceneManager::GetInstance()->selectedEntity.get();
        auto camera = SceneManager::GetInstance()->activeCamera.get();
        if (renderer->drawTransformGizmos && entity != nullptr && camera != nullptr && entity->getType() != EntityType::Material)
        {
            // Manipulate the transform of this selected entity
            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
            static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
            if (ImGui::IsKeyPressed(ImGuiKey_T, false))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(ImGuiKey_R, false))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(ImGuiKey_E, false))
                mCurrentGizmoOperation = ImGuizmo::SCALE;

            if (ImGui::IsKeyPressed(ImGuiKey_L, false))
                if (mCurrentGizmoMode == ImGuizmo::WORLD)
                    mCurrentGizmoMode = ImGuizmo::LOCAL;
                else
                    mCurrentGizmoMode = ImGuizmo::WORLD;

            ImGuizmo::Manipulate(glm::value_ptr(camera->GetViewMatrix()), glm::value_ptr(camera->GetProjectionMatrix(new_width, new_height)),
                                 mCurrentGizmoOperation, mCurrentGizmoMode, (float *)glm::value_ptr(entity->getTransform()));
        }

        if (!ImGuizmo::IsUsing() && ImGui::IsItemHovered() && io->MouseDown[0])
        {
            auto pos = SceneManager::GetInstance()->activeCamera;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                SceneManager::GetInstance()->activeCamera->ProcessKeyboard(FORWARD, io->DeltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                SceneManager::GetInstance()->activeCamera->ProcessKeyboard(BACKWARD, io->DeltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                SceneManager::GetInstance()->activeCamera->ProcessKeyboard(LEFT, io->DeltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                SceneManager::GetInstance()->activeCamera->ProcessKeyboard(RIGHT, io->DeltaTime);

            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.1f);
                SceneManager::GetInstance()->activeCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
                ImGui::ResetMouseDragDelta();
            }
        }
    }
    ImGui::End();

    ImGuiWindowClass c;
    c.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&c);
    if (ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoTitleBar))
    {
        // toolsPanel(addLightButtonTex);
    }
    ImGui::End();

    // Scene Hierarchy and Properties windows on the left
    if (ImGui::Begin("Scene Hierarchy"))
    {
        layOutSceneHierarchy();
    }
    ImGui::End();

    if (ImGui::Begin("Properties"))
    {
        displayPropertiesOfEntity(SceneManager::GetInstance()->selectedEntity);
    }
    ImGui::End();

    if (ImGui::Begin("Materials"))
    {
        layoutMaterials();
    }
    ImGui::End();

    if (ImGui::Begin("Console"))
    {
        ConsoleLogWindow();
    }
    ImGui::End();

    if (ImGui::Begin("Content Browser"))
    {
    }
    ImGui::End();

    if (ImGui::Begin("Settings"))
    {
        rasterizerSettings(renderer.get());
    }
    ImGui::End();

    // if (ImGui::Begin("FPS"))
    // {
    //     std::string s(std::to_string(io->Framerate));
    //     ImGui::Text(s.c_str());

    //     // auto material = SceneManager::GetInstance()->materials[0];
    //     // if (ImGui::Button("Mark material for preview"))
    //     // {
    //     //     MaterialPreview::GetInstance()->markForPreview(material);
    //     // }
    //     // glViewport(0, 0, MaterialPreview::PREVIEW_SIZE, MaterialPreview::PREVIEW_SIZE);
    //     // ImGui::Image(
    //     //     (ImTextureID)material->previewTexture,
    //     //     ImVec2(MaterialPreview::PREVIEW_SIZE, MaterialPreview::PREVIEW_SIZE),
    //     //     ImVec2(0, 1),
    //     //     ImVec2(1, 0));
    //     // MaterialPreview::GetInstance()->renderPreviews();
    // }
    // ImGui::End();

    MaterialPreview::GetInstance()->renderPreviews();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

int Window::Init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    if (!glfwInit())
        return -1;

    this->window = glfwCreateWindow(this->windowData.width, this->windowData.height, this->title.c_str(), NULL, NULL);

    if (!window)
    {
        LOG_CRITICAL("Fatal error. Failed to create window with GLFW");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (this->vsyncEnabled)
    {
        LOG_INFO("Vsync enabled");
        glfwSwapInterval(1);
    }
    else
    {
        LOG_WARN("Vsync is disabled");
        glfwSwapInterval(0);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL("Fatal error. Failed to load GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, &(this->windowData));

    glfwSetWindowSizeCallback(this->window,
                              [](GLFWwindow *window, int width, int height)
                              {
                                  WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);
                                  data->width = width;
                                  data->height = height;

                                  ResizeWindowEvent event(width, height);
                                  data->eventFn(event);
                              });

    glfwSetCursorPosCallback(window,
                             [](GLFWwindow *window, double x, double y)
                             {
                                 WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);

                                 MouseMovedEvent event(x, y);
                                 data->eventFn(event);
                             });

    glfwSetWindowCloseCallback(window,
                               [](GLFWwindow *window)
                               {
                                   WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);

                                   CloseWindowEvent event;
                                   data->eventFn(event);
                               });

    LOG_INFO("Successfully created window");
    initialized = true;
    return 0;
}

void Window::Shutdown()
{
    initialized = false;
    LOG_INFO("Shutting down ImGui...");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    LOG_INFO("Successfully destroyed window");
}

void ConsoleLogWindow()
{
    const float bottom_space = ImGui::GetStyle().ItemSpacing.y;
    if (ImGui::BeginChild("ScrollRegion##", ImVec2(0, -bottom_space), false, 0))
    {
        // Wrap items.
        ImGui::PushTextWrapPos();

        std::vector<ConsoleLine> console_entries = Console::GetInstance()->getContents();

        for (int i = 0; i < console_entries.size(); i++)
        {
            ConsoleLine line = console_entries[i];
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            if (line.entry_type == SuccesEntry)
                color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            if (line.entry_type == LogEntry)
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            if (line.entry_type == WarningEntry)
                color = ImVec4(1.0f, 0.75f, 0.0f, 1.0f);
            if (line.entry_type == ErrorEntry)
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(line.text.c_str());
            ImGui::PopStyleColor();
        }

        // Auto-scroll logs.
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
}

void embraceTheDarkness()
{
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;
}
