#include <array>
#include <cerrno>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iostream>

constexpr unsigned short SONY_VID = 0x054C;
constexpr unsigned short DUALSENSE_PID = 0x0CE6;

int main() {
  hid_device *controller = hid_open(SONY_VID, DUALSENSE_PID, nullptr);

  if (controller == nullptr) {
    int err = errno;

    const wchar_t *hid_err = hid_error(nullptr);

    if (hid_err) {
      std::wcerr << L"hid_error:" << hid_err << L'\n';
    }

    std::cerr << "errno: " << strerror(err) << '\n';

    hid_exit();
    return 1;
  }

  std::array<unsigned char, 128> report{};

  return 0;
}
