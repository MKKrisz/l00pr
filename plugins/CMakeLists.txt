add_library(Vesztettem MODULE vesztettem.hpp vesztettem.cpp)
add_library(Cosine MODULE cosine.hpp cosine.cpp)

target_link_libraries(Vesztettem l00pr_core)
target_link_libraries(Cosine l00pr_core)
