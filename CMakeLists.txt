cmake_minimum_required(VERSION 3.10)
project(libMesh)

add_library(libMesh mesh.cpp vector.cpp point.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
