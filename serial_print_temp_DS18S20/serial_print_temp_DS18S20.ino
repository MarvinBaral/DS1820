const int BUS_PIN = 10;
const int NUM_OF_SENSORS = 5;

//comands (transmit with least significant bit first!!!):
const boolean SEARCH_ROM[8] = {0,0,0,0,1,1,1,1}; //0xF0
const boolean READ_ROM[8] = {1,1,0,0,1,1,0,0}; //0x33
const boolean MATCH_ROM[8] = {1,0,1,0,1,0,1,0}; //0x55
const boolean SKIP_ROM[8] = {0,0,1,1,0,0,1,1}; //0xCC
const boolean ALARM_SEARCH[8] = {0,0,1,1,0,1,1,1}; //0xEC
const boolean CONVERT_T[8] = {0,0,1,0,0,0,1,0}; //0x44
const boolean WRITE_SCRATCHPAD[8] = {0,1,1,1,0,0,1,0}; //0x4E
const boolean READ_SCRATCHPAD[8] = {0,1,1,1,1,1,0,1}; //0xBE
const boolean COPY_SCRATCHPAD[8] = {0,0,0,1,0,0,1,0}; //0x48
const boolean RECALL_E[8] = {0,0,0,1,1,1,0,1}; //0xB8
const boolean READ_POWER[8] = {0,0,1,0,1,1,0,1}; //0xB4
const boolean ROM_CODES[NUM_OF_SENSORS][64] = {
  {0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0},
  {0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
  {0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0},
  {0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0},
  {0,0,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1}
};

//timing
const int PULLDOWN_TIME = 3; //us
const int TEMP_CONVERT_TIME = 1; //ms

//reset only
const int MIN_TIME_RESET_PULSE = 480; //us
const int MAX_TIME_PRESENCE_PULSE = 240; //us
const int MIN_TIME_MASTER_RESET_SAMPLING = 480; //us
const int MAX_DELAY_SENSOR = 60; //us

boolean input = 0;
unsigned long startMicros = 0;
unsigned long timeMicros = 0;
boolean data[NUM_OF_SENSORS][72]; //array where every set of read data is saved

void setup() {
  Serial.begin(9600);
  Serial.println("started");
}

void loop() {
  getTemp(BUS_PIN, 0);
  getTemp(BUS_PIN, 1);
  getTemp(BUS_PIN, 2);
  getTemp(BUS_PIN, 3);
  getTemp(BUS_PIN, 4);
  //printROMSingle(BUS_PIN, true);
  Serial.println();
  delay(1000);
}

boolean reset(int pBusPin) { //can even be executed multiple times directly after each other without problems
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, 0);
  delayMicroseconds(MIN_TIME_RESET_PULSE);
  
  pinMode(pBusPin, INPUT_PULLUP);
  startMicros = micros();
  delayMicroseconds(MAX_DELAY_SENSOR); //In this time there is a peak when master releases bus and after this sensor pulls bus down
  do{
    input = digitalRead(pBusPin);
    timeMicros = micros() - startMicros;
  } while(!input || timeMicros > (MIN_TIME_MASTER_RESET_SAMPLING + 50));
  
  bool success = (timeMicros > MAX_DELAY_SENSOR && timeMicros < MAX_TIME_PRESENCE_PULSE);
  if (!success) {
    Serial.println("No sensor responding to ping.");
    Serial.print("Time between master releasing bus and sensor releasing bus: ");
    Serial.print(timeMicros);
    Serial.println(" us");
  }
  return success;
}

void writeBus(int pBusPin, boolean logicLevel) {
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, LOW);
  delayMicroseconds(PULLDOWN_TIME);
  if(logicLevel){
    pinMode(pBusPin, INPUT_PULLUP);
  }
  delayMicroseconds(60);
  if(!logicLevel){
    pinMode(pBusPin, INPUT_PULLUP);
  }
  delayMicroseconds(1);
}

boolean readBus(int pBusPin) {
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, LOW);
  delayMicroseconds(PULLDOWN_TIME);
  pinMode(pBusPin, INPUT_PULLUP);
  delayMicroseconds(15 - PULLDOWN_TIME - 5);
  boolean input = digitalRead(pBusPin);
  delayMicroseconds(50); //wait for read time slot to be over (minimum 60us)

  return input;
}
void getROMSingle(int pBusPin) {
  reset(BUS_PIN);
  delayMicroseconds(100);
  writeCommand(BUS_PIN, READ_ROM);
  delayMicroseconds(5); //!!!!!!!!!!!!!!!!!!!!!!!!
  readData(BUS_PIN, 64, 0);
}

void printROMSingle(int pBusPin, boolean toPasteIntoROMCodes_h) {
  getROMSingle(pBusPin);
  for (int i = 0; i < 64; i++) {
    Serial.print(data[0][i]);
    if (toPasteIntoROMCodes_h) {
      Serial.print(",");
    }
  }
  Serial.println();
}

void writeCommand(int pBusPin, const boolean pCommand[8]) {
  writeData(pBusPin, 8, pCommand);
}

void writeData(int pBusPin, const int numBits, const boolean pData[]) {
  for(int i = 0; i < numBits; i++){
    writeBus(pBusPin, pData[i]);
  }  
}

void readData(int pBusPin, const int numBits, int indexSensor) {
  for(int q = 0; q < numBits; q++) {
    data[indexSensor][q] = readBus(pBusPin);
  }
}

void getTempSingle(int pBusPin) {
  reset(BUS_PIN);
  delayMicroseconds(100);
  writeCommand(BUS_PIN, SKIP_ROM);
  writeCommand(BUS_PIN, CONVERT_T);
  delay(TEMP_CONVERT_TIME); //wait for temperature conversion to finish
  reset(BUS_PIN);
  delayMicroseconds(100);
  writeCommand(BUS_PIN, SKIP_ROM);
  writeCommand(BUS_PIN, READ_SCRATCHPAD);
  delayMicroseconds(5);
  readData(BUS_PIN, 8, 0);
  reset(BUS_PIN); //suppress sending of further unnecessary information
  Serial.println(convertArrayToTemp(data, 0));
}

void getTemp(int pBusPin, int indexSensor) {
  reset(pBusPin);
  delayMicroseconds(100);
  writeCommand(pBusPin, MATCH_ROM);
  writeData(pBusPin, 64, ROM_CODES[indexSensor]);
  writeCommand(BUS_PIN, CONVERT_T);
  delay(TEMP_CONVERT_TIME); //wait for temperature conversion to finish
  reset(BUS_PIN);
  delayMicroseconds(100);
  writeCommand(pBusPin, MATCH_ROM);
  writeData(pBusPin, 64, ROM_CODES[indexSensor]);
  writeCommand(BUS_PIN, READ_SCRATCHPAD);
  delayMicroseconds(5);
  readData(BUS_PIN, 8, indexSensor);
  reset(BUS_PIN); //suppress sending of further unnecessary information
  float temp = convertArrayToTemp(data, indexSensor);
  Serial.print(indexSensor);
  Serial.print(": ");
  if (temp < 125) { //default value if only the high bus is read instead of sensor-data
    Serial.println(temp);
  } else {
    Serial.println("error, unrealistic high value");
  }
}

float convertArrayToTemp(boolean array[][72], int indexSensor) {
  float temp = 0;
  for (int i = 0; i < 8; i++) {
    temp += array[indexSensor][i] * pow(2, (i - 1));    
  }
  return temp;
}


