# CMakeLists.txt for Dear ImGui

# Add all the source files of ImGui
file(GLOB_RECURSE IMGUI_SOURCES "*.cpp")

# Create the ImGui library
add_library(imgui ${IMGUI_SOURCES})

# Include the directory with ImGui headers
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backends)

find_package(glfw3 REQUIRED)
target_link_libraries(imgui PUBLIC glfw)

find_package(GLEW REQUIRED)
target_link_libraries(imgui PUBLIC GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC OpenGL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
