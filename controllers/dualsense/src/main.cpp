#include "adapter.h"
#include "interpreter.h"
#include "serial.h"
#include <iostream>

constexpr char output_device[] = "/dev/ttyACM0";

int main() {
  int ret = interpret_init();

  if (ret != 0) {
    std::cerr << "Error on interpret_init";
    return ret;
  }

  int fd = -1;

  ret = serial_open(output_device, fd);

  if (ret != 0) {
    std::cerr << "Error on open_serial_port";
    return ret;
  }

  ret = serial_configure(fd, B115200);

  if (ret != 0) {
    std::cerr << "Error on configure_serial_port";
    return ret;
  }

  dualsense_state state{};

  while (true) {
    ret = interpret_next_report(state);

    if (ret == 0) {
      continue;
    }

    auto command = adapt_dualsense_state_to_ms_command(state);

    ret = serial_write_byte(fd, command);

    if (ret != 0) {
      std::cerr << "Error on serial_write_byte";
      return ret;
    }
  }

  interpret_close();

  return 0;
}
