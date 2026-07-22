#pragma once

#include <cstdint>
#include <termios.h>

int serial_open(const char *device, int &fd);
int serial_configure(int fd, speed_t baud_rate);
int serial_write_data(int fd, const std::uint8_t *data, std::size_t size);
int serial_write_byte(int fd, const std::uint8_t byte);
