#include "one_wire_lib.h"

const int BUS_PIN = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("started");
}

void loop() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    getTemp(BUS_PIN, i);
  }
  if (Serial.available()) {
    Serial.print("got: ");
    Serial.println(Serial.read());
  }
  //printROMSingle(BUS_PIN, true);
}
