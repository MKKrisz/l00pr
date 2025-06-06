enable_testing()
target_sources(${PROJECT_NAME} PUBLIC device.hpp
                                audiodevice.hpp audiodevice.cpp
                                renderdevice.hpp renderdevice.cpp
                                printerdevice.hpp printerdevice.cpp
)
