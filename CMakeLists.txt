cmake_minimum_required(VERSION 3.20)
project(l00pr)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp 
                               audiosource.h
                               concepts.hpp
                               device.h device.cpp
                               generator.hpp generator.cpp
                               interpolated.hpp interpolated.cpp
                               note.hpp note.cpp
                               noteplayer.hpp noteplayer.cpp
                               generators/builtin.hpp
                               generators/none.hpp generators/none.cpp
                               generators/sine.hpp generators/sine.cpp
                               generators/square.hpp generators/square.cpp
                               generators/triangle.hpp generators/triangle.cpp
                               generators/register.hpp generators/register.cpp
                               notestream.hpp notestream.cpp
                               util.hpp util.cpp
                               tune.hpp tune.cpp
                               exceptions/parse_error.hpp exceptions/parse_error.cpp)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
