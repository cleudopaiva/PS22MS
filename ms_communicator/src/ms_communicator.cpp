#include "ms_communicator.h"
#include "ms_command.h"
#include <Arduino.h>
#include <map>

const int D_PAD_UP_PIN = D9;
const int D_PAD_DOWN_PIN = D8;
const int D_PAD_LEFT_PIN = D7;
const int D_PAD_RIGHT_PIN = D6;
const int RESET_PIN = D5;
const int BUTTON_1_PIN = D4;
const int BUTTON_PAUSE_PIN = D3;
const int BUTTON_2_PIN = D2;

std::map<int, int> button_maps = {{1, D_PAD_UP_PIN},     {2, D_PAD_DOWN_PIN},
                                  {3, D_PAD_LEFT_PIN},   {4, D_PAD_RIGHT_PIN},
                                  {5, RESET_PIN},        {6, BUTTON_1_PIN},
                                  {7, BUTTON_PAUSE_PIN}, {8, BUTTON_2_PIN}};

void setup_ms_pins() {
  pinMode(D_PAD_UP_PIN, OUTPUT);
  pinMode(D_PAD_DOWN_PIN, OUTPUT);
  pinMode(D_PAD_LEFT_PIN, OUTPUT);
  pinMode(D_PAD_RIGHT_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(BUTTON_1_PIN, OUTPUT);
  pinMode(BUTTON_PAUSE_PIN, OUTPUT);
  pinMode(BUTTON_2_PIN, OUTPUT);
}

void write_button_state(uint8_t value, uint8_t mask, uint8_t pin) {
  digitalWrite(pin, (value & mask) ? HIGH : LOW);
}

void handle_serial_commands() {
  if (Serial.available() > 0) {
    uint8_t received_byte = static_cast<uint8_t>(Serial.read());

    write_button_state(received_byte, master_system_mask::up, D_PAD_UP_PIN);
    write_button_state(received_byte, master_system_mask::down, D_PAD_DOWN_PIN);
    write_button_state(received_byte, master_system_mask::right,
                       D_PAD_RIGHT_PIN);
    write_button_state(received_byte, master_system_mask::left, D_PAD_LEFT_PIN);
    write_button_state(received_byte, master_system_mask::btn_1, BUTTON_1_PIN);
    write_button_state(received_byte, master_system_mask::btn_2, BUTTON_2_PIN);
    write_button_state(received_byte, master_system_mask::start,
                       BUTTON_PAUSE_PIN);
    write_button_state(received_byte, master_system_mask::reset, RESET_PIN);
  }
}
