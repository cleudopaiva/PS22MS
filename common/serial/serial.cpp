#include "serial.h"
#include "fcntl.h"
#include <iostream>
#include <unistd.h>

int serial_open(const char *device, int &fd) {
  fd = open(device, O_RDWR | O_NOCTTY);

  if (fd == -1) {
    std::cerr << "open_serial_port_error: failed to open serial port";
    return 1;
  }

  return 0;
}

int serial_configure(int fd, speed_t baud_rate) {
  termios tty{};

  if (tcgetattr(fd, &tty) == -1) {
    std::cerr << "configure_serial_port_error: Failed to read serial port "
                 "configuration\n";
    return 1;
  }

  cfmakeraw(&tty);

  if (cfsetispeed(&tty, baud_rate) == -1 ||
      cfsetospeed(&tty, baud_rate) == -1) {
    std::cerr << "configure_serial_port_error: Failed to set baud_rate\n";
    return 1;
  }

  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  tty.c_cflag |= CLOCAL | CREAD;

  if (tcsetattr(fd, TCSANOW, &tty) == -1) {
    std::cerr << "configure_serial_port_error: Failed to apply serial port "
                 "configuration";
    return 1;
  }

  return 0;
}

int serial_write_data(int fd, const std::uint8_t *data, std::size_t size) {
  std::size_t total_written = 0;

  while (total_written < size) {
    const ssize_t bytes_written =
        ::write(fd, data + total_written, size - total_written);

    if (bytes_written == -1) {
      if (errno == EINTR) {
        continue;
      }

      std::cerr << "write_serial_data_error: Failed to write to serial port";
      return 1;
    };

    if (bytes_written == 0) {
      std::cerr
          << "write_serial_data_error: Serial port write returned zero bytes";
      return 1;
    }

    total_written += static_cast<std::size_t>(bytes_written);
  }

  return 0;
}

int serial_write_byte(int fd, const std::uint8_t byte) {
  while (true) {
    const ssize_t bytes_written = ::write(fd, &byte, sizeof(byte));

    if (bytes_written == -1) {
      if (errno == EINTR) {
        continue;
      }

      std::cerr
          << "write_serial_data_error: Failed to write to the serial port";
      return 1;
    }

    if (bytes_written == 0) {
      std::cerr
          << "write_serial_data_error: Serial prot write returned zero bytes";
      return 1;
    }

    return 0;
  }
}
