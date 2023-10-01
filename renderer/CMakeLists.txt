cmake_minimum_required(VERSION 3.14)
project(Renderer)

find_package(OpenGL REQUIRED)

add_library(Renderer renderer.cpp)

### GLFW ###

find_package(glfw3 REQUIRED)

target_link_libraries(Renderer glfw OpenGL)
