cmake_minimum_required(VERSION 3.13)
project(cellular_evolution)

include_directories(./include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(bitmap-write-example examples/bitmap_write/main.cpp  examples/bitmap_write/main.cpp)
add_executable(evolution-example examples/evolution_simple/main.cpp examples/evolution_simple/main.cpp examples/evolution_simple/test_ind_data.h examples/evolution_simple/test_ind.h)
