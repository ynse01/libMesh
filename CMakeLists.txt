cmake_minimum_required(VERSION 3.14)
project(Mesh)

add_library(Mesh mesh.cpp vector2.cpp vector3.cpp point2.cpp point3.cpp transformation.cpp meshFactory.cpp plane.cpp siedel.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(yaui)
add_subdirectory(test)
