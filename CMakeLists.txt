include(FetchContent)
include(CMakePrintHelpers)

set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.5)
project(rasterizer)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a build directory
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
file(MAKE_DIRECTORY ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# Define the root directory of the project
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
# Pass the root directory as a preprocessor definition
add_compile_definitions(PROJECT_ROOT_DIR="${PROJECT_ROOT_DIR}")

# Source files here
file(GLOB_RECURSE SRC_FILES
    src/*.h
    src/*.cpp
    src/*.hpp
)

include_directories(
	src
	src/cameras
	src/entities
	src/events
	src/framebuffer
	src/gui
	src/gui/Panels
	src/lights
	src/materials
	src/renderers
	external
)

# Add the external libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/external/imgui)
add_subdirectory(external/glad)
# add_subdirectory(external/spdlog)

# Add the source files
set(SRCS ${SRC_FILES})

# Add the executable
add_executable(rasterizer ${SRCS})

# ------------------  Link external libraries-------------------------

# -------------------------- SPDLOG -------------------------------------
# target_link_libraries(rasterizer PUBLIC spdlog)

# -------------------------- GLM -------------------------------------
include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)
target_link_libraries(rasterizer PUBLIC glm::glm)

# ---------------------------ImGui -----------------------------------
target_link_libraries(rasterizer PUBLIC imgui)


# ---------------------------- GLFW --------------------------------
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)
target_link_libraries(rasterizer PUBLIC glfw)

# ------------------------- GLAD ----------------
target_include_directories(rasterizer PUBLIC external/glad/include)
target_link_libraries(rasterizer PUBLIC glad)

# --------------------------- ASSIMP ----------------
set(ASSIMP_BUILD_TESTS OFF)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.4.2 # Specify the version or branch you want to use
)
FetchContent_MakeAvailable(assimp)
target_link_libraries(rasterizer PUBLIC assimp)