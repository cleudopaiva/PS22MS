#include <array>
#include <hidapi/hidapi.h>
#include <iostream>

constexpr unsigned short SONY_VID = 0x054C;
constexpr unsigned short DUALSENSE_PID = 0x0CE6;

int main() {
  if (hid_init() != 0) {
    std::cerr << "Error initializing HIDAPI\n";
    return 1;
  }

  hid_device *controller = hid_open(SONY_VID, DUALSENSE_PID, nullptr);

  if (controller == nullptr) {
    std::cerr << "Dualsense not found or without permition\n";
    hid_exit();
    return 1;
  }

  std::array<unsigned char, 128> report{};
}
