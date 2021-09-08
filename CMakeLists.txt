cmake_minimum_required(VERSION 3.19)
project(Plotter)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

find_library(SDL2_LIBRARY NAME SDL2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

set(Plotter_SRC main.cpp plot/graph/Graph.cpp plot/graph/GraphPrimitive.cpp plot/graph/GraphContent.cpp plot/sdl/GfxExtensions.cpp plot/vector/Vector.cpp plot/vector/Vector.h plot/vector/Matrix3x3.cpp plot/vector/Matrix3x3.h)

add_executable(Plotter "${Plotter_SRC}")
include_directories(Plotter ${SDL2_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS})
target_link_libraries(Plotter ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_GFX_LIBRARY})