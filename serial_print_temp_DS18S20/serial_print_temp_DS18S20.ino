#include "one_wire_lib.h"

const int BUS_PIN = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("started");
}

void loop() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    requestTemp(BUS_PIN, i);
  }
  delay(TEMP_CONVERT_TIME); //wait for temperature conversion to finish
  for (int i = 0; i < NUM_SENSORS; i++) {
    float temp = readTemp(BUS_PIN, i);
    printTemp(i, temp);
  }
  if (Serial.available()) {
    Serial.print("got: ");
    Serial.println(Serial.read());
  }
  //printROMSingle(BUS_PIN, true);
}
