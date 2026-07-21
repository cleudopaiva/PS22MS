#include "ms_communicator.h"
#include <Arduino.h>

void setup() {
  setup_ms_pins();
  Serial.begin(9600);
}

void loop() {
  handle_serial_commands();
  delay(100);
}
