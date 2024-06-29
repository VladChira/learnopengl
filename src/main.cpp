#include <iostream>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION

#include "openglcontext/opengl_context.hpp"

int init_gui();
void init_frame();
void after_frame();

GLFWwindow *window;
ImGuiIO *io;
ImVec4 clear_color(0.0f, 0.0f, 0.0f, 1.0f);
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

float cameraPos[3];
float cameraRot[2];

int main()
{
	int ret = init_gui();
	if (ret != 0)
		return -1;

	OpenGLContext opengl((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
	opengl.init();

	while (!glfwWindowShouldClose(window))
	{
		init_frame();

		int window_width, window_height;
		glfwGetWindowSize(window, &window_width, &window_height);

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)));
		if (ImGui::Begin("OpenGL", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar))
		{
			ImVec2 avail = ImGui::GetContentRegionAvail();

			// Define the aspect ratio of your scene
			float aspect_ratio = 16.0f / 9.0f; // Example: 16:9 aspect ratio

			// Calculate the new dimensions preserving the aspect ratio
			float new_width = avail.x;
			float new_height = avail.x / aspect_ratio;

			if (new_height > avail.y)
			{
				new_height = avail.y;
				new_width = avail.y * aspect_ratio;
			}

			// Resize the framebuffer and set the viewport
			opengl.sceneBuffer.RescaleFrameBuffer(new_width, new_height);
			glViewport(0, 0, new_width, new_height);
			opengl.render();

			// Draw the texture with the calculated size
			unsigned int tex = opengl.getFrameBufferTexture();
			ImGui::Image(
				(ImTextureID)tex,
				ImVec2(new_width, new_height),
				ImVec2(0, 1),
				ImVec2(1, 0));

			if (ImGui::IsItemHovered())
			{
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
					opengl.camera.ProcessKeyboard(FORWARD, io->DeltaTime);
				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
					opengl.camera.ProcessKeyboard(BACKWARD, io->DeltaTime);
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
					opengl.camera.ProcessKeyboard(LEFT, io->DeltaTime);
				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
					opengl.camera.ProcessKeyboard(RIGHT, io->DeltaTime);

				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.1f);
					opengl.camera.ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
					ImGui::ResetMouseDragDelta();
				}
			}
		}
		ImGui::End();

		// ImGui::SetNextWindowPos(ImVec2(static_cast<float>(window_width) * 0.8, static_cast<float>(window_height) * 0.05));
		// ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width) * 0.2, static_cast<float>(window_height) * 0.1));
		ImGui::Begin("Debug Info", nullptr);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
		ImGui::End();

		// ImGui::SetNextWindowPos(ImVec2(static_cast<float>(window_width) * 0.8, static_cast<float>(window_height) * 0.15));
		// ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width) * 0.2, static_cast<float>(window_height) * 0.2));
		ImGui::Begin("Controls", nullptr);

		if (ImGui::CollapsingHeader("Camera Controls"))
		{
			cameraPos[0] = opengl.camera.Position.x;
			cameraPos[1] = opengl.camera.Position.y;
			cameraPos[2] = opengl.camera.Position.z;
			if (ImGui::InputFloat3("Camera Position", cameraPos))
			{
				opengl.camera.Position.x = cameraPos[0];
				opengl.camera.Position.y = cameraPos[1];
				opengl.camera.Position.z = cameraPos[2];
			}

			cameraRot[0] = opengl.camera.Pitch;
			cameraRot[1] = opengl.camera.Yaw;
			if (ImGui::SliderFloat2("Camera Angles", cameraRot, -90, 90))
			{
				opengl.camera.Pitch = cameraRot[0];
				opengl.camera.Yaw = cameraRot[1];
				opengl.camera.updateCameraVectors();
			}
		}

		ImGui::ColorEdit3("Clear Color", opengl.clear_color);
		ImGui::InputFloat4("Phong Settings", opengl.phongConstants);

		if (ImGui::CollapsingHeader("Point Light Settings"))
		{
			ImGui::SliderFloat3("Point Light Position", opengl.pointPos, -5.0, 5.0);
			ImGui::ColorEdit3("Point Light Color", opengl.pointColor);
		}

		if (ImGui::CollapsingHeader("Directional Light Settings"))
		{
			ImGui::SliderFloat3("Directional Light Direction", opengl.dirPos, -1.0, 1.0);
			ImGui::ColorEdit3("Directional Light Color", opengl.dirColor);
		}

		if (ImGui::CollapsingHeader("Spot Light Settings"))
		{
			ImGui::ColorEdit3("Spot Light Color", opengl.spotColor);
		}

		ImGui::End();

		after_frame();
	}

	glfwTerminate();
	return 0;
}

void init_frame()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void after_frame()
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

int init_gui()
{
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL & ImGUI", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew initialisation failed!\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return 0;
}
