#include "printerdevice.hpp"

void PrinterDevice::start() {
    for(const auto& tune : tunes) {
        tune->Write(std::cout);
        std::cout << std::endl << std::endl;
    }
    Device::stop();
}

void PrinterDevice::stop() {/*nothing*/}
