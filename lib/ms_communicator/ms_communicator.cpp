#include "ms_communicator.h"
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

void handle_serial_commands() {
  if (Serial.available() > 0) {
    int number = Serial.parseInt();

    Serial.println(number);

    auto button_map = button_maps.find(number);

    if (button_map != button_maps.end()) {
      int pin = button_map->second;

      digitalWrite(pin, HIGH);
      delay(50);
      digitalWrite(pin, LOW);
    }
  }
}
