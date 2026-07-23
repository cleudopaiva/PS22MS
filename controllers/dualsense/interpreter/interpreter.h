#pragma once

#include <cstdint>
#include <cstring>
#include <hidapi/hidapi.h>

constexpr unsigned short SONY_VID = 0x054C;
constexpr unsigned short DUALSENSE_PID = 0x0CE6;
constexpr unsigned long long HID_READ_TIMEOUT_IN_MS = 100;
constexpr unsigned int DUALSENSE_INPUT_REPORT_USB_ID = 0x01;
constexpr unsigned int DUALSENSE_INPUT_REPORT_USB_SIZE = 64;
constexpr unsigned int DUALSENSE_BASIC_INPUT_REPORT_BT_ID = 0x01;
constexpr unsigned int DUALSENSE_BASIC_INPUT_REPORT_BT_SIZE = 10;
constexpr unsigned int DUALSENSE_FULL_INPUT_REPORT_BT_ID = 0x32;
constexpr unsigned int DUALSENSE_FULL_INPUT_REPORT_BT_SIZE = 78;

namespace dualsense_state_mask {
constexpr uint8_t square = 1u << 4;
constexpr uint8_t cross = 1u << 5;
constexpr uint8_t circle = 1u << 6;
constexpr uint8_t triangle = 1u << 7;
constexpr uint8_t l1 = 1u << 0;
constexpr uint8_t r1 = 1u << 1;
constexpr uint8_t l2 = 1u << 2;
constexpr uint8_t r2 = 1u << 3;
constexpr uint8_t create = 1u << 4;
constexpr uint8_t options = 1u << 5;
constexpr uint8_t l3 = 1u << 6;
constexpr uint8_t r3 = 1u << 7;
constexpr uint8_t ps = 1u << 0;
constexpr uint8_t touchpad = 1u << 1;
constexpr uint8_t microphone = 1u << 2;
} // namespace dualsense_state_mask

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

void decode_hat(std::uint8_t value, dualsense_state &state);

int parse_usb_report(const std::uint8_t *report, std::size_t size);
int interpret_init();
int interpret_next_report(dualsense_state &state);
void interpret_close();
