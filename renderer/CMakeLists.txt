cmake_minimum_required(VERSION 3.14)
project(Renderer)

find_package(OpenGL REQUIRED)

add_library(Renderer renderer.cpp)

### GLFW ###
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(glfw3 REQUIRED)

### Glad ###
# Generated using: https://glad.dav1d.de/
set(GLAD_DIR "../deps/glad")
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")

target_link_libraries(Renderer glfw glad OpenGL::GL)
