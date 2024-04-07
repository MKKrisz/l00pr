cmake_minimum_required(VERSION 3.20)
project(l00pr)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
