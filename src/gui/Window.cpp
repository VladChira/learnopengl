#include "Window.hpp"

#include <iostream>

const char *glsl_version = "#version 130";

Window::Window(std::string title, const unsigned int width, const unsigned int height, bool enableVsync)
{
    this->title = title;
    this->windowData.width = width;
    this->windowData.height = height;
    this->initialized = false;
    this->vsyncEnabled = enableVsync;
    this->Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    std::cout << "Successfully initialzed ImGui.\n";
}

Window::~Window()
{
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

    // Draw the GUI here
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
    // Set up the docking space

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
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);

        // Split the left dock for Scene Hierarchy and Properties
        ImGuiID dock_left_top_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Up, 0.5f, nullptr, &dock_left_id);
        ImGuiID dock_left_bottom_id = dock_left_id;

        // Split the right dock for Panel 1 and Panel 2
        ImGuiID dock_right_top_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Up, 0.5f, nullptr, &dock_right_id);
        ImGuiID dock_right_bottom_id = dock_right_id;

        // Dock windows
        ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_left_top_id);
        ImGui::DockBuilderDockWindow("Properties", dock_left_bottom_id);
        ImGui::DockBuilderDockWindow("Materials", dock_bottom_id);
        ImGui::DockBuilderDockWindow("Content Browser", dock_bottom_id);
        ImGui::DockBuilderDockWindow("Stats", dock_right_top_id);
        ImGui::DockBuilderDockWindow("Settings", dock_right_bottom_id);
        ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

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
        ImGui::EndMenuBar();
    }
    ImGui::EndMainMenuBar();

    // Scene Hierarchy and Properties windows on the left
    if (ImGui::Begin("Scene Hierarchy"))
    {
        // Placeholder for scene hierarchy contents
    }
    ImGui::End();

    if (ImGui::Begin("Properties"))
    {
        // Placeholder for properties contents
    }
    ImGui::End();

    // Materials and Content Browser at the bottom (can switch between the two)
    if (ImGui::Begin("Materials"))
    {
        // Placeholder for materials contents
    }
    ImGui::End();

    if (ImGui::Begin("Content Browser"))
    {
        // Placeholder for content browser contents
    }
    ImGui::End();

    // Panels on the right
    if (ImGui::Begin("Stats"))
    {
        // Placeholder for first panel contents
    }
    ImGui::End();

    if (ImGui::Begin("Settings"))
    {
        // Placeholder for second panel contents
    }
    ImGui::End();

    if (ImGui::Begin("Viewport"))
    {
        // Placeholder for second panel contents
    }
    ImGui::End();

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
        std::cout << "Fatal error. Failed to create window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (this->vsyncEnabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Fatal error. GLEW initialisation failed!\n";
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

    std::cout << "Successfully initialzed GLFW, GLEW and created window.\n";
    initialized = true;
    return 0;
}

void Window::Shutdown()
{
    initialized = false;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Destroying window\n";
}