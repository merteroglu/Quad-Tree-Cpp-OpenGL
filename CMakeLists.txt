cmake_minimum_required(VERSION 3.6)
project(MyQuadTree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Vertex.h QTNode.h QuadTree.h QuadTree.cpp)
add_executable(MyQuadTree ${SOURCE_FILES} Vertex.h QTNode.h QuadTree.h QuadTree.cpp)