
enable_testing()

target_sources(${PROJECT_NAME} PUBLIC audiosource.hpp audiosource.cpp audiosource_test.cpp
                               concepts.hpp
                               device.h device.cpp
                               interpolated.hpp interpolated_test.cpp
                               freq.hpp freq.cpp freq_test.cpp
                               range.hpp range_test.cpp
                               util.hpp util.cpp util_test.cpp
                               argmgr.hpp argmgr.cpp
                               pstate.hpp pstate.cpp
                               string_convertible.hpp
                               parseable_base.hpp parseable_test.cpp)
add_subdirectory(exceptions)
add_subdirectory(generator)
add_subdirectory(player)
add_subdirectory(tune)
add_subdirectory(filter)

