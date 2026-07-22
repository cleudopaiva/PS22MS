#pragma once

#include <cstdint>
#include <cstring>
#include <hidapi/hidapi.h>
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

void decode_hat(std::uint8_t value, dualsense_state &state);

std::optional<dualsense_state> parse_usb_report(const std::uint8_t *report,
                                                std::size_t size);
int interpret_init();
std::optional<dualsense_state> interpret_next();
void interpret_close();
