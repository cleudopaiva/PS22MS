#include "ms_communicator.h"
#include <Arduino.h>

void setup() {
  setup_ms_pins();
  Serial.begin(115200);
}

void loop() {
  handle_serial_commands();
  delay(100);
}
