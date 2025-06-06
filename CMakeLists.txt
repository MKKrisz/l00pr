cmake_minimum_required(VERSION 3.25)
project(l00pr_core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JPORTA)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


find_package(SDL2 REQUIRED)

#enable_testing()

add_library(${PROJECT_NAME} SHARED l00pr.hpp)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2 GTest::gtest GTest::gtest_main)

project(l00pr)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 l00pr_core)

add_subdirectory(plugins)
#include(GoogleTest)
#gtest_discover_tests(${PROJECT_NAME})
