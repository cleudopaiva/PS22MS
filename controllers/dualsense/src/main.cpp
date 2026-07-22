#include "adapter.h"
#include "interpreter.h"
#include <iostream>

int main() {
  int ret = interpret_init();

  if (ret != 0) {
    std::cerr << "interpret_init_error";
    return ret;
  }

  while (true) {
    auto state = interpret_next();

    if (!state.has_value()) {
      continue;
    }

    auto command = adapt_dualsense_state_to_ms_command(state.value());

    std::cout << "0x" << std::hex << +command << "\n";
  }

  interpret_close();

  return 0;
}
