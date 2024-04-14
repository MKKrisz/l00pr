target_sources(${PROJECT_NAME} PUBLIC audiosource.h
                               concepts.hpp
                               device.h device.cpp
                               interpolated.hpp interpolated.cpp
                               freq.hpp freq.cpp
                               util.hpp util.cpp)
add_subdirectory(exceptions)
add_subdirectory(generator)
add_subdirectory(player)
add_subdirectory(tune)
