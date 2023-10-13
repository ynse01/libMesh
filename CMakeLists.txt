cmake_minimum_required(VERSION 3.14)
project(Mesh)

add_library(Mesh mesh.cpp vector.cpp point.cpp transformation.cpp meshExtruder.cpp plane.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(renderer)
add_subdirectory(test)
