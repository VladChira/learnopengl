include(FetchContent)
include(CMakePrintHelpers)

set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 3.5)
project(test)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a build directory
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
file(MAKE_DIRECTORY ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})


# Source files here
file(GLOB_RECURSE SRC_FILES
    src/*.h
    src/*.cpp
    src/*.hpp
)

include_directories(
	src/gui
	src/cameras
	src/framebuffer
	src/openglcontext
)

# Add the external libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/external/imgui)

# Add the source files
set(SRCS ${SRC_FILES})

# Add the executable
add_executable(test ${SRCS})

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

# Link external libraries

target_link_libraries(test PRIVATE glm::glm)

target_link_libraries(test PRIVATE imgui)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/external/tinyobjloader)

find_package(glfw3 REQUIRED)
target_link_libraries(test PUBLIC glfw)

find_package(GLEW REQUIRED)
target_link_libraries(test PUBLIC GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(test PUBLIC OpenGL)

find_package(assimp REQUIRED)
target_link_libraries(test PRIVATE assimp::assimp)