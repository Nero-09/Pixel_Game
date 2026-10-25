cmake_minimum_required(VERSION 3.16)
project(PixelGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(game ${SOURCES})
target_link_libraries(game raylib)
