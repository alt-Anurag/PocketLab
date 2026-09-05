#include <Arduino.h>

const int potPin = 34;
const int dacPin = 25;

// 0 = Sine 
// 1 = Sawtooth
// 2 = Triangle
// 3 = Square
int waveType = 0;

#define TABLE_SIZE 256
#define MIDPOINT 128
#define AMPLITUDE 120

uint8_t sineTable[TABLE_SIZE];

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < TABLE_SIZE; i++) {
    float angle = (2.0 * PI * i) / TABLE_SIZE;
    sineTable[i] = MIDPOINT + AMPLITUDE * sin(angle);
  }

  Serial.println("ESP32 Function Generator Ready");
}

void loop() {
  int potValue = analogRead(potPin);
  int delayTime = map(potValue, 0, 4095, 3, 120);

  switch (waveType) {
    case 0: runSine(delayTime); break;
    case 1: runSawtooth(delayTime); break;
    case 2: runTriangle(delayTime); break;
    case 3: runSquare(delayTime); break;
  }
}

void runSine(int d) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    dacWrite(dacPin, sineTable[i]);
    delayMicroseconds(d);
  }
}

void runSawtooth(int d) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    uint8_t val = MIDPOINT - AMPLITUDE + (2 * AMPLITUDE * i) / TABLE_SIZE;
    dacWrite(dacPin, val);
    delayMicroseconds(d);
  }
}

void runTriangle(int d) {
  for (int i = 0; i < TABLE_SIZE / 2; i++) {
    uint8_t val = MIDPOINT - AMPLITUDE + (4 * AMPLITUDE * i) / TABLE_SIZE;
    dacWrite(dacPin, val);
    delayMicroseconds(d);
  }

  for (int i = TABLE_SIZE / 2; i < TABLE_SIZE; i++) {
    uint8_t val = MIDPOINT + AMPLITUDE - (4 * AMPLITUDE * (i - TABLE_SIZE / 2)) / TABLE_SIZE;
    dacWrite(dacPin, val);
    delayMicroseconds(d);
  }
}

void runSquare(int d) {
  dacWrite(dacPin, MIDPOINT + AMPLITUDE);
  delayMicroseconds(d * 128);

  dacWrite(dacPin, MIDPOINT - AMPLITUDE);
  delayMicroseconds(d * 128);
}
