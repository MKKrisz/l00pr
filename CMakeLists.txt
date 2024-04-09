cmake_minimum_required(VERSION 3.20)
project(l00pr)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
