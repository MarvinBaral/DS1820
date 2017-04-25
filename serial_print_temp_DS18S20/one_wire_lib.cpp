#include <Arduino.h>
#include "one_wire_lib.h"
#include "rom_codes.h"

bool input = 0;
bool data[72]; //array where every set of read data is saved

void writeCommand(int pBusPin, const bool pCommand[8]) {
  writeData(pBusPin, 8, pCommand);
}

bool reset(int pBusPin) { //can even be executed multiple times directly after each other without problems
  unsigned long startMicros = 0;
  unsigned long timeMicros = 0;
  
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, 0);
  delayMicroseconds(MIN_TIME_RESET_PULSE);
  
  pinMode(pBusPin, INPUT);
  startMicros = micros();
  delayMicroseconds(RESET_MAX_DELAY_SENSOR); //In this time there is a peak when master releases bus and after this sensor pulls bus down
  do{
    input = digitalRead(pBusPin);
    timeMicros = micros() - startMicros;
  } while(!input || timeMicros > MIN_TIME_MASTER_RESET_SAMPLING);
  
  bool success = (timeMicros > RESET_MAX_DELAY_SENSOR && timeMicros < MIN_TIME_MASTER_RESET_SAMPLING); //MAX_TIME_PRESENCE_PULSE * 1.1
  if (!success) {
    Serial.println("No sensor responding to ping.");
    Serial.print("Time between master releasing bus and sensor releasing bus: ");
    Serial.print(timeMicros);
    Serial.println(" us");
  }
  return success;
}

void writeBus(int pBusPin, bool logicLevel) {
  delayMicroseconds(RECOVERY_BETWEEN_SLOTS);
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, LOW);
  delayMicroseconds(PULLDOWN_TIME);
  if(logicLevel){
    pinMode(pBusPin, INPUT);
  }
  delayMicroseconds(MIN_SLOT_SIZE);
  if(!logicLevel){
    pinMode(pBusPin, INPUT);
  }
}

bool readBus(int pBusPin) {
  delayMicroseconds(RECOVERY_BETWEEN_SLOTS);
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, LOW);
  delayMicroseconds(PULLDOWN_TIME);
  pinMode(pBusPin, INPUT);
  delayMicroseconds(READ_SAMPLE_TIME - PULLDOWN_TIME + READ_OFFSET);
  bool input = digitalRead(pBusPin);
  delayMicroseconds(MIN_SLOT_SIZE - READ_SAMPLE_TIME -RECOVERY_BETWEEN_SLOTS); //wait for read time slot to be over (minimum 60us)
  return input;
}
void getROMSingle(int pBusPin) {
  reset(pBusPin);
  delayMicroseconds(100);
  writeCommand(pBusPin, READ_ROM);
  delayMicroseconds(5); //!!!!!!!!!!!!!!!!!!!!!!!!
  readData(pBusPin, 64);
}

void printROMSingle(int pBusPin, bool toPasteIntoROMCodes_h) {
  getROMSingle(pBusPin);
  for (int i = 0; i < 64; i++) {
    Serial.print(data[i]);
    if (toPasteIntoROMCodes_h) {
      Serial.print(",");
    }
  }
  Serial.println();
}

void writeData(int pBusPin, const int numBits, const bool pData[]) {
  for(int i = 0; i < numBits; i++){
    writeBus(pBusPin, pData[i]);
  }  
}

void readData(int pBusPin, const int numBits) {
  for(int q = 0; q < numBits; q++) {
    data[q] = readBus(pBusPin);
  }
}

void getTempSingle(int pBusPin) {
  reset(pBusPin);
  writeCommand(pBusPin, SKIP_ROM);
  writeCommand(pBusPin, CONVERT_T);
  delay(TEMP_CONVERT_TIME); //wait for temperature conversion to finish
  reset(pBusPin);
  writeCommand(pBusPin, SKIP_ROM);
  writeCommand(pBusPin, READ_SCRATCHPAD);
  delayMicroseconds(5);
  readData(pBusPin, READ_BYTES_FOR_TEMP);
  reset(pBusPin); //suppress sending of further unnecessary information
  Serial.println(convertArrayToTemp(data));
}

float getTemp(int pBusPin, int indexSensor) {
  requestTemp(pBusPin, indexSensor);
  delay(TEMP_CONVERT_TIME); //wait for temperature conversion to finish
  float temp = readTemp(pBusPin, indexSensor);
  printTemp(indexSensor, temp); //to be activateable by param
  return temp;
}

void requestTemp(int pBusPin, int indexSensor) {
  reset(pBusPin);
  writeCommand(pBusPin, MATCH_ROM);
  writeData(pBusPin, 64, ROM_CODES[indexSensor]);
  writeCommand(pBusPin, CONVERT_T);
}

float readTemp(int pBusPin, int indexSensor) {
  reset(pBusPin);
  writeCommand(pBusPin, MATCH_ROM);
  writeData(pBusPin, 64, ROM_CODES[indexSensor]);
  writeCommand(pBusPin, READ_SCRATCHPAD);
  delayMicroseconds(5);
  readData(pBusPin, READ_BYTES_FOR_TEMP);
  reset(pBusPin); //suppress sending of further unnecessary information
  return convertArrayToTemp(data);  
}

void printTemp(int indexSensor, float temp) {
  Serial.print(indexSensor);
  Serial.print(": ");
  Serial.println(temp);
}

float convertArrayToTemp(bool array[72]) {
  float temp = 0;
  bool signByteIsOk = true;
  signed short int negativityFactor = 1;
  
  for (int i = 8; i < 16; i++) {
      if (array[i] != array[8]) {
        signByteIsOk = false;
      }
  }
  //invert to negative
  if (signByteIsOk && (array[8] == true)) {
    negativityFactor = -1;
    //invert
    for (int i = 0; i < 8; i++) {
      array[i] = !array[i];   
    }
    //add 1
    for (int i = 0; i < 8; i++) {
      if (array[i] == true) {
        array[i] = false;
      } else {
        array[i] = true;
        break;
      }
    }
  } else if (!signByteIsOk) {
    Serial.print("error: sign Byte not equal");
  }
  
  for (int i = 0; i < 8; i++) {  
    temp += array[i] * pow(2, (i - 1));    
  }
  return temp * negativityFactor;
}
