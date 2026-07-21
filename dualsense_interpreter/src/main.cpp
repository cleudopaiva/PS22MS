#include <array>
#include <cerrno>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iostream>

constexpr unsigned short SONY_VID = 0x054C;
constexpr unsigned short DUALSENSE_PID = 0x0CE6;
constexpr unsigned long long HID_READ_TIMEOUT_IN_MS = 100;

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

  int exit_code = 0;

  while (true) {
    const int bytes_read =
        hid_read_timeout(controller, report.data(), report.size(), 100);

    if (bytes_read < 0) {
      const wchar_t *hid_err = hid_read_error(controller);

      if (hid_err) {
        std::wcerr << L"hid_read_error:" << hid_err << L'\n';
      }

      exit_code = 1;
      break;
    }

    if (bytes_read == 0) {
      continue;
    }
  }

  hid_close(controller);
  hid_exit();

  return 0;
}
