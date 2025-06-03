
#enable_testing()

target_sources(${PROJECT_NAME} PUBLIC audiosource.hpp audiosource.cpp audiosource_test.cpp
                               concepts.hpp
                               interpolated.hpp interpolated_test.cpp
                               freq.hpp freq.cpp freq_test.cpp
                               range.hpp range_test.cpp
                               util.hpp util.cpp util_test.cpp
                               arg.hpp arg.cpp
                               argmgr.hpp argmgr.cpp
                               pstate.hpp pstate.cpp
                               plugin.hpp plugin.cpp
                               plugin_mgr.hpp plugin_mgr.cpp
                               string_convertible.hpp
                               parseable_base.hpp parseable_test.cpp)
add_subdirectory(device)
add_subdirectory(exceptions)
add_subdirectory(generator)
add_subdirectory(player)
add_subdirectory(tune)
add_subdirectory(filter)

