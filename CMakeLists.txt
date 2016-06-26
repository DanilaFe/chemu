cmake_minimum_required(VERSION 3.5.2)
project(chemu)

file(GLOB SOURCES "src/*.c")

add_executable(chemu ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(glfw3 3.2 REQUIRED)

include_directories(include)
include_directories(chemu ${OPENGL_INCLUDE_DIR})

target_link_libraries(chemu glfw)
target_link_libraries(chemu ${OPENGL_glu_LIBRARY})
