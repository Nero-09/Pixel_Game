cmake_minimum_required(VERSION 3.16)

project(PixelGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/map/*.cpp
    # src/player/*.cpp
)

add_executable(game ${SOURCES})

target_link_libraries(game PRIVATE raylib)