#include <array>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iostream>
#include <optional>

#include "interpreter.h"

hid_device *controller = nullptr;

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
  case 8:
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

int interpret_init() {
  controller = hid_open(SONY_VID, DUALSENSE_PID, nullptr);

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

  return 0;
}

std::optional<dualsense_state> interpret_next() {
  if (controller == nullptr) {
    std::cerr
        << "interpret_error: interpret_next was called before interpret_init";
    return std::nullopt;
  }

  std::array<unsigned char, 128> report{};

  const int bytes_read =
      hid_read_timeout(controller, report.data(), report.size(), 100);

  if (bytes_read < 0) {
    const wchar_t *hid_err = hid_read_error(controller);

    if (hid_err) {
      std::wcerr << L"hid_read_error:" << hid_err << L'\n';
    }

    return std::nullopt;
  }

  // The value is 0 if no data was present within the timeout
  if (bytes_read == 0) {
    return std::nullopt;
  }

  std::printf("Report 0x%02X size %d \n", report[0], bytes_read);

  auto state = parse_usb_report(report.data(), bytes_read);

  if (!state.has_value()) {
    std::cerr << "hid_report_error: the report was in an unkown format \n";
    return std::nullopt;
  }

  return state;
}

void interpret_close() {
  if (controller != nullptr) {
    hid_close(controller);
    controller = nullptr;
  }

  hid_exit();
}
