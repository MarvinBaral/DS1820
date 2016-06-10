const int BUS_PIN = 10;
const int NUM_SENSORS = 15;

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

boolean input = 0;
unsigned long startMicros = 0;
unsigned long timeMicros = 0;
boolean data[72]; //array where every set of read data is saved

void setup() {
  Serial.begin(9600);
  Serial.println("started");
}

void loop() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    getTemp(BUS_PIN, i);
  }
  //printROMSingle(BUS_PIN, true);
}

boolean reset(int pBusPin) { //can even be executed multiple times directly after each other without problems
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, 0);
  delayMicroseconds(MIN_TIME_RESET_PULSE);
  
  pinMode(pBusPin, INPUT_PULLUP);
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

void writeBus(int pBusPin, boolean logicLevel) {
  delayMicroseconds(RECOVERY_BETWEEN_SLOTS);
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, LOW);
  delayMicroseconds(PULLDOWN_TIME);
  if(logicLevel){
    pinMode(pBusPin, INPUT_PULLUP);
  }
  delayMicroseconds(MIN_SLOT_SIZE);
  if(!logicLevel){
    pinMode(pBusPin, INPUT_PULLUP);
  }
}

boolean readBus(int pBusPin) {
  delayMicroseconds(RECOVERY_BETWEEN_SLOTS);
  pinMode(pBusPin, OUTPUT);
  digitalWrite(pBusPin, LOW);
  delayMicroseconds(PULLDOWN_TIME);
  pinMode(pBusPin, INPUT_PULLUP);
  delayMicroseconds(READ_SAMPLE_TIME - PULLDOWN_TIME + READ_OFFSET);
  boolean input = digitalRead(pBusPin);
  delayMicroseconds(MIN_SLOT_SIZE - READ_SAMPLE_TIME -RECOVERY_BETWEEN_SLOTS); //wait for read time slot to be over (minimum 60us)
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
    Serial.print(data[i]);
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
    data[q] = readBus(pBusPin);
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
  Serial.println(convertArrayToTemp(data));
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
  float temp = convertArrayToTemp(data);
  Serial.print(indexSensor);
  Serial.print(": ");
  if (temp < 125) { //default value if only the high bus is read instead of sensor-data
    Serial.println(temp);
  } else {
    Serial.println("error, unrealistic high value");
  }
}

float convertArrayToTemp(boolean array[72]) {
  float temp = 0;
  for (int i = 0; i < 8; i++) {
    temp += array[i] * pow(2, (i - 1));    
  }
  return temp;
}


