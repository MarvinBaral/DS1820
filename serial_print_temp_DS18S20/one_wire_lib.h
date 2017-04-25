#ifndef ONE_WIRE_LIB_H
#define ONE_WIRE_LIB_H

#include "rom_codes.h" //you may want to use NUM_SENSORS in main sketch file

//comands (transmit with least significant bit first!!!):
const bool SEARCH_ROM[8] = {0,0,0,0,1,1,1,1}; //0xF0
const bool READ_ROM[8] = {1,1,0,0,1,1,0,0}; //0x33
const bool MATCH_ROM[8] = {1,0,1,0,1,0,1,0}; //0x55
const bool SKIP_ROM[8] = {0,0,1,1,0,0,1,1}; //0xCC
const bool ALARM_SEARCH[8] = {0,0,1,1,0,1,1,1}; //0xEC
const bool CONVERT_T[8] = {0,0,1,0,0,0,1,0}; //0x44
const bool WRITE_SCRATCHPAD[8] = {0,1,1,1,0,0,1,0}; //0x4E
const bool READ_SCRATCHPAD[8] = {0,1,1,1,1,1,0,1}; //0xBE
const bool COPY_SCRATCHPAD[8] = {0,0,0,1,0,0,1,0}; //0x48
const bool RECALL_E[8] = {0,0,0,1,1,1,0,1}; //0xB8
const bool READ_POWER[8] = {0,0,1,0,1,1,0,1}; //0xB4

//timing
const int PULLDOWN_TIME = 3; //us, min 1 us
const int TEMP_CONVERT_TIME = 750; //ms
const int RECOVERY_BETWEEN_SLOTS = 3; //us min 1 us
const int READ_SAMPLE_TIME = 15; //us
const signed int  READ_OFFSET = -5; //us
const int MIN_SLOT_SIZE = 60; //us


//reset only
const int MIN_TIME_RESET_PULSE = 480; //us
const int MAX_TIME_PRESENCE_PULSE = 240; //us
const int MIN_TIME_MASTER_RESET_SAMPLING = 480; //us
const int RESET_MAX_DELAY_SENSOR = 60; //us

const int READ_BYTES_FOR_TEMP = 16;


//==========================================================================

void writeCommand(int pBusPin, const bool pCommand[8]);
bool reset(int pBusPin);
void writeBus(int pBusPin, bool logicLevel);
bool readBus(int pBusPin);
void printROMSingle(int pBusPin, bool toPasteIntoROMCodes_h);
void writeData(int pBusPin, const int numBits, const bool pData[]);
void readData(int pBusPin, const int numBits);
void getTempSingle(int pBusPin);
float getTemp(int pBusPin, int indexSensor);
void requestTemp(int pBusPin, int indexSensor);
float readTemp(int pBusPin, int indexSensor);
void printTemp(int indexSensor, float temp);
float convertArrayToTemp(bool array[72]);

#endif

