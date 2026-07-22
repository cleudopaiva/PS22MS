#include "adapter.h"

namespace {
constexpr std::uint8_t ANALOG_LOW_THRESHOLD = 100;
constexpr std::uint8_t ANALOG_HIGH_THRESHOLD = 156;
} // namespace

master_system_command
adapt_dualsense_state_to_ms_command(const dualsense_state &state) {

  const bool analog_left = state.left_x <= ANALOG_LOW_THRESHOLD;
  const bool analog_right = state.left_x >= ANALOG_HIGH_THRESHOLD;
  const bool analog_up = state.left_y <= ANALOG_LOW_THRESHOLD;
  const bool analog_down = state.left_y >= ANALOG_HIGH_THRESHOLD;

  master_system_command command = 0;

  if (state.up || state.left_y <= 100) {
    command |= master_system_mask::up;
  }

  if (state.down || state.left_y >= 156) {
    command |= master_system_mask::down;
  }

  if (state.left || state.left_x <= 100) {
    command |= master_system_mask::left;
  }

  if (state.right || state.left_x >= 156) {
    command |= master_system_mask::right;
  }

  if (state.cross) {
    command |= master_system_mask::btn_1;
  }

  if (state.square) {
    command |= master_system_mask::btn_2;
  }

  if (state.options || state.create) {
    command |= master_system_mask::start;
  }

  if (state.ps)
    command |= master_system_mask::reset;

  return command;
}
