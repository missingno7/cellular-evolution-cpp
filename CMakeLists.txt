cmake_minimum_required(VERSION 3.13)
project(cellular_evolution)

include_directories(./include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(bitmap-write-example examples/bitmap_write/main.cpp  examples/bitmap_write/main.cpp)
add_executable(simple-evolution-example examples/evolution_simple/main.cpp examples/evolution_simple/main.cpp)
add_executable(floor-planning-example examples/floor_planning/main.cpp)
add_executable(salesman-example examples/salesman/main.cpp)
add_executable(expression-tree-example examples/expression_tree/main.cpp)
add_executable(img-shape-example examples/image_shapes/main.cpp)
add_executable(coverage-example examples/coverage/main.cpp)
