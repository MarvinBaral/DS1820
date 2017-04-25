#ifndef ROM_CODES_H
#define ROM_CODES_H

//modify these values for your need, printROMSingle(int pBusPin, true) outputs rom-codes in the format to copy&paste them in here

const int NUM_SENSORS = 15;
const boolean ROM_CODES[NUM_SENSORS][64] = {
  {0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0},
  {0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
  {0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0},
  {0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0},
  {0,0,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1},
  {0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0},
  {0,0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,0,0,1,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
  {0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0},
  {0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0},
  {0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
  {0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,0,0,0,1,1,1,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1},
  {0,0,0,0,1,0,0,0,1,1,0,1,1,1,0,0,1,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1},
  {0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0}
};

#endif