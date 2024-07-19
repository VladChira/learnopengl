# CMakeLists.txt for Dear ImGui

cmake_minimum_required(VERSION 3.5)

# Add all the source files of ImGui
file(GLOB_RECURSE IMGUI_SOURCES "*.cpp")

# Create the ImGui library
add_library(imgui ${IMGUI_SOURCES})

# Include the directory with ImGui headers
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backends)

include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)
target_link_libraries(imgui PUBLIC glfw)

# find_package(OpenGL REQUIRED)
# target_link_libraries(imgui PUBLIC OpenGL)

target_link_libraries(imgui PUBLIC glad)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
