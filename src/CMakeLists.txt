cmake_minimum_required(VERSION 2.6)
project(opengltest)

# Load SDL

#set(BOX2D_DIR, "/usr/lib/Box2D")

include(FindPkgConfig)
find_package(OpenGL REQUIRED)
find_package(Box2D REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)

file(GLOB SOURCES *.cpp)

add_definitions(-std=c++11)
add_definitions(-Wall -Werror)
add_executable(opengltest ${SOURCES})
target_link_libraries(opengltest ${SDL2_LIBRARIES}) 
target_link_libraries(opengltest ${OPENGL_LIBRARIES})
target_link_libraries(opengltest Box2D)