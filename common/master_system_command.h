#pragma once

#include <cstdint>

using master_system_command = std::uint8_t;

namespace master_system_mask {
constexpr master_system_command up = 1u << 0;
constexpr master_system_command down = 1u << 1;
constexpr master_system_command left = 1u << 2;
constexpr master_system_command right = 1u << 3;
constexpr master_system_command btn_1 = 1u << 4;
constexpr master_system_command btn_2 = 1u << 5;
constexpr master_system_command start = 1u << 6;
constexpr master_system_command reset = 1u << 7;
} // namespace master_system_mask
