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
add_library(libl00pr STATIC l00pr.hpp
                            src/audiosource.hpp
                            src/concepts.hpp
                            src/device.h src/device.cpp
                            src/freq.hpp src/freq.cpp
                            src/interpolated.hpp
                            src/interpolator.hpp
                            src/range.hpp
                            src/util.hpp src/util.cpp
                            src/exceptions/parse_error.hpp src/exceptions/parse_error.cpp
                            src/generator/builtin.hpp
                            src/generator/generator.hpp src/generator/generator.cpp
                            src/generator/none.hpp src/generator/none.cpp
                            src/generator/register.hpp src/generator/register.cpp
                            src/generator/sine.hpp src/generator/sine.cpp
                            src/generator/square.hpp src/generator/square.cpp
                            src/generator/triangle.hpp src/generator/triangle.cpp
                            src/player/note.hpp src/player/note.cpp
                            src/player/noteplayer.hpp src/player/noteplayer.cpp
                            src/player/random_note.hpp src/player/random_note.cpp
                            src/player/setnote.hpp src/player/setnote.cpp
                            src/tune/lane.hpp
                            src/tune/loop.hpp src/tune/loop.cpp
                            src/tune/notestream.hpp src/tune/notestream.cpp
                            src/tune/tune.hpp src/tune/tune.cpp)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
