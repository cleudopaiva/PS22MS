#include <array>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iostream>

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

int parse_dualsense_report(const std::uint8_t *report, std::size_t size,
                           dualsense_state &state) {
  if (report == nullptr) {
    std::cerr << "parse_dualsense_report_error: the report was null";
    return 1;
  }

  // First byte is the report ID
  int payload_offest;

  switch (report[0]) {
  case DUALSENSE_BASIC_INPUT_REPORT_BT_ID:
  case DUALSENSE_INPUT_REPORT_USB_SIZE:
    if (size != DUALSENSE_INPUT_REPORT_USB_SIZE &&
        size != DUALSENSE_BASIC_INPUT_REPORT_BT_SIZE) {
      std::cerr << "parse_dualsense_report_error: the report has a wrong size";
      return 1;
    }

    payload_offest = 1;
    break;
  case DUALSENSE_FULL_INPUT_REPORT_BT_ID:
    if (size != DUALSENSE_FULL_INPUT_REPORT_BT_SIZE) {
      std::cerr << "parse_dualsense_report_error: the report has a wrong size";
      return 1;
    }

    // Full BT reports use the second byte as header sequence
    payload_offest = 2;
  default:

    std::cerr << "parse_dualsense_report_error: the report id is uknown: "
              << report[0];
    return 1;
  }

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

  state.square = buttons0 & dualsense_state_mask::square;
  state.cross = buttons0 & dualsense_state_mask::cross;
  state.circle = buttons0 & dualsense_state_mask::circle;
  state.triangle = buttons0 & dualsense_state_mask::triangle;

  state.l1 = buttons1 & dualsense_state_mask::l1;
  state.r1 = buttons1 & dualsense_state_mask::r1;
  state.l2 = buttons1 & dualsense_state_mask::l2;
  state.r2 = buttons1 & dualsense_state_mask::r2;
  state.create = buttons1 & dualsense_state_mask::create;
  state.options = buttons1 & dualsense_state_mask::options;
  state.l3 = buttons1 & dualsense_state_mask::l3;
  state.r3 = buttons1 & dualsense_state_mask::r3;

  state.ps = buttons2 & dualsense_state_mask::ps;
  state.touchpad = buttons2 & dualsense_state_mask::touchpad;
  state.microphone = buttons2 & dualsense_state_mask::microphone;

  return 0;
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

int interpret_next_report(dualsense_state &state) {
  if (controller == nullptr) {
    std::cerr
        << "interpret_error: interpret_next was called before interpret_init";
    return 1;
  }

  std::array<unsigned char, 128> report{};

  const int bytes_read =
      hid_read_timeout(controller, report.data(), report.size(), 100);

  if (bytes_read < 0) {
    const wchar_t *hid_err = hid_read_error(controller);

    if (hid_err) {
      std::wcerr << L"hid_read_error:" << hid_err << L'\n';
    }

    return 1;
  }

  // The value is 0 if no data was present within the timeout
  if (bytes_read == 0) {
    return 0;
  }

  std::printf("Report 0x%02X size %d \n", report[0], bytes_read);

  return parse_dualsense_report(report.data(), bytes_read, state);
}

void interpret_close() {
  if (controller != nullptr) {
    hid_close(controller);
    controller = nullptr;
  }

  hid_exit();
}
