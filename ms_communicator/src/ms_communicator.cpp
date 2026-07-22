#include "ms_communicator.h"
#include "ms_command.h"
#include <Arduino.h>

const int D_PAD_UP_PIN = D9;
const int D_PAD_DOWN_PIN = D8;
const int D_PAD_LEFT_PIN = D7;
const int D_PAD_RIGHT_PIN = D6;
const int RESET_PIN = D5;
const int BUTTON_1_PIN = D4;
const int BUTTON_PAUSE_PIN = D3;
const int BUTTON_2_PIN = D2;

constexpr unsigned long COMMAND_STALE_TIMEOUT_MS = 100;
unsigned long last_command_received_at_ms = 0;
uint8_t active_master_system_command = 0;

void setup_ms_pins() {
  pinMode(D_PAD_UP_PIN, OUTPUT);
  digitalWrite(D_PAD_UP_PIN, LOW);

  pinMode(D_PAD_DOWN_PIN, OUTPUT);
  digitalWrite(D_PAD_DOWN_PIN, LOW);

  pinMode(D_PAD_LEFT_PIN, OUTPUT);
  digitalWrite(D_PAD_LEFT_PIN, LOW);

  pinMode(D_PAD_RIGHT_PIN, OUTPUT);
  digitalWrite(D_PAD_RIGHT_PIN, LOW);

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);

  pinMode(BUTTON_1_PIN, OUTPUT);
  digitalWrite(BUTTON_1_PIN, LOW);

  pinMode(BUTTON_PAUSE_PIN, OUTPUT);
  digitalWrite(BUTTON_PAUSE_PIN, LOW);

  pinMode(BUTTON_2_PIN, OUTPUT);
  digitalWrite(BUTTON_2_PIN, LOW);
}

void write_button_state(uint8_t value, uint8_t mask, uint8_t pin) {
  digitalWrite(pin, (value & mask) ? HIGH : LOW);
}

void write_master_system_command(uint8_t command) {
  write_button_state(command, master_system_mask::up, D_PAD_UP_PIN);
  write_button_state(command, master_system_mask::down, D_PAD_DOWN_PIN);
  write_button_state(command, master_system_mask::right, D_PAD_RIGHT_PIN);
  write_button_state(command, master_system_mask::left, D_PAD_LEFT_PIN);
  write_button_state(command, master_system_mask::btn_1, BUTTON_1_PIN);
  write_button_state(command, master_system_mask::btn_2, BUTTON_2_PIN);
  write_button_state(command, master_system_mask::start, BUTTON_PAUSE_PIN);
  write_button_state(command, master_system_mask::reset, RESET_PIN);
}

void handle_serial_commands() {
  uint8_t latest_serial_command = active_master_system_command;

  bool has_command = false;

  while (Serial.available() > 0) {
    latest_serial_command = static_cast<uint8_t>(Serial.read());
    has_command = true;
  }

  if (has_command) {
    active_master_system_command = latest_serial_command;
    last_command_received_at_ms = millis();
    write_master_system_command(active_master_system_command);
  }

  if (active_master_system_command != 0 &&
      millis() - last_command_received_at_ms > COMMAND_STALE_TIMEOUT_MS) {
    active_master_system_command = 0;
    write_master_system_command(active_master_system_command);
  }
}
