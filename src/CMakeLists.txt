
enable_testing()

target_sources(${PROJECT_NAME} PUBLIC audiosource.hpp audiosource.cpp
                               concepts.hpp
                               device.h device.cpp
                               interpolated.hpp
                               freq.hpp freq.cpp
                               range.hpp
                               util.hpp util.cpp util_test.cpp
                               argmgr.hpp argmgr.cpp
                               pstate.hpp pstate.cpp
                               string_convertible.hpp
                               parseable_base.hpp)
add_subdirectory(exceptions)
add_subdirectory(generator)
add_subdirectory(player)
add_subdirectory(tune)
add_subdirectory(filter)

