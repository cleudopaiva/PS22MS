#include <SPI.h>

const int SS_PIN = SS;

byte defaultCommand[5] = {0x01, 0x42, 0x00, 0x00, 0x00};

void setup() {
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  Serial.begin(9600);
  SPI.begin();
  SPI.beginTransaction(SPISettings(250000, LSBFIRST, SPI_MODE3));
}

void loop() {
  byte data[5];

  digitalWrite(SS_PIN, LOW);

  for (int i = 0; i < 5; i++) {
    byte dataByte = SPI.transfer(defaultCommand[i]);
    Serial.print(dataByte, HEX);
    Serial.print("");                          
    data[i] = dataByte;
  }

  Serial.print("\n");

  digitalWrite(SS_PIN, HIGH);

  delay(50);
}
                                                                            
