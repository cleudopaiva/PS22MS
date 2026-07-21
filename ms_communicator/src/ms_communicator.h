#pragma once

#include <Arduino.h>

extern const int D_PAD_UP_PIN;
extern const int D_PAD_DOWN_PIN;
extern const int D_PAD_LEFT_PIN;
extern const int D_PAD_RIGHT_PIN;
extern const int RESET_PIN;
extern const int BUTTON_1_PIN;
extern const int BUTTON_PAUSE_PIN;
extern const int BUTTON_2_PIN;

void setup_ms_pins();
void handle_serial_commands();
