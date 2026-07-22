#include <array>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iostream>
#include <optional>

constexpr unsigned short SONY_VID = 0x054C;
constexpr unsigned short DUALSENSE_PID = 0x0CE6;
constexpr unsigned long long HID_READ_TIMEOUT_IN_MS = 100;

struct dualsense_state {
  bool up{};
  bool down{};
  bool left{};
  bool right{};

  bool square{};
  bool cross{};
  bool circle{};
  bool triangle{};

  bool l1{};
  bool r1{};
  bool l2{};
  bool r2{};

  bool create{};
  bool options{};
  bool l3{};
  bool r3{};

  bool ps{};
  bool touchpad{};
  bool microphone{};

  std::uint8_t left_x{};
  std::uint8_t left_y{};
  std::uint8_t right_x{};
  std::uint8_t right_y{};
  std::uint8_t l2_analog{};
  std::uint8_t r2_analog{};
};

void decode_hat(std::uint8_t value, dualsense_state &state) {
  const std::uint8_t hat = value & 0x0F;

  switch (hat) {
  case 0:
    state.up = true;
    break;
  case 1:
    state.up = true;
    state.right = true;
    break;
  case 2:
    state.right = true;
    break;
  case 3:
    state.right = true;
    state.down = true;
    break;
  case 4:
    state.down = true;
    break;
  case 5:
    state.down = true;
    state.left = true;
    break;
  case 6:
    state.left = true;
    break;
  case 7:
    state.left = true;
    state.up = true;
    break;
  }
}

std::optional<dualsense_state> parse_usb_report(const std::uint8_t *report,
                                                std::size_t size) {
  if (report == nullptr || size < 64) {
    return std::nullopt;
  }

  if (report[0] != 0x01) {
    return std::nullopt;
  }

  dualsense_state state;

  state.left_x = report[1];
  state.left_y = report[2];
  state.right_x = report[3];
  state.right_y = report[4];

  state.l2_analog = report[5];
  state.r2_analog = report[6];

  const std::uint8_t buttons0 = report[8];
  const std::uint8_t buttons1 = report[9];
  const std::uint8_t buttons2 = report[10];

  decode_hat(buttons0, state);

  state.square = buttons0 & (1u << 4);
  state.cross = buttons0 & (1u << 5);
  state.circle = buttons0 & (1u << 6);
  state.triangle = buttons0 & (1u << 7);

  state.l1 = buttons1 & 1u;
  state.r1 = buttons1 & (1u << 1);
  state.l2 = buttons1 & (1u << 2);
  state.r2 = buttons1 & (1u << 3);
  state.create = buttons1 & (1u << 4);
  state.options = buttons1 & (1u << 5);
  state.l3 = buttons1 & (1u << 6);
  state.r3 = buttons1 & (1u << 7);

  state.ps = buttons2 & 1u;
  state.touchpad = buttons2 & (1u << 1);
  state.microphone = buttons2 & (1u << 2);

  return state;
}

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

    // The value is 0 if no data was present within the timeout
    if (bytes_read == 0) {
      continue;
    }

    std::printf("Report 0x%02X size %d: ", report[0], bytes_read);

    parse_usb_report(report.data(), bytes_read);
  }

  hid_close(controller);
  hid_exit();

  return 0;
}
