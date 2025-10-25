cmake_minimum_required(VERSION 3.10)
project(ogl_test VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(GLEW REQUIRED)        # will create GLEW::GLEW imported target
find_package(OpenGL REQUIRED)      # creates OpenGL::GL

add_executable(ogl_test main.cpp)

target_include_directories(ogl_test PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(ogl_test PRIVATE ${GLFW_LIBRARIES} GLEW::GLEW OpenGL::GL)
