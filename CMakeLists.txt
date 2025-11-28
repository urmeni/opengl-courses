cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)  # Prefer GLVND
project(ogl_test VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable debug symbols if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

find_package(glm REQUIRED)

# Include directories
include_directories(
        ${GLFW_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)


# Collect all source files automatically
file(GLOB_RECURSE SRC_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/main.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLFW_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
        ${GLM_INCLUDE_DIRS}
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${GLFW_LIBRARIES}
        GLEW::GLEW
        OpenGL::GL
)

# Copy resource folders (models, textures, shaders) to build dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
)
