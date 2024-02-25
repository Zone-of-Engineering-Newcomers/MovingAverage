#include "HardwareSerial.h"
#include "MovingAverage.h"

MovingAverage::MovingAverage() {}

MovingAverage::~MovingAverage() {}

void MovingAverage::begin() {
  this->enabled = true;
}

void MovingAverage::end() {
  this->enabled = false;
}

void MovingAverage::print() {
  while (!Serial) {}

  Serial.print("Raw-Data:");
  Serial.print(this->input);
  Serial.print("\tSMA:");
  Serial.print(this->sma_output);
  Serial.print("\tCA:");
  Serial.println(this->ca_output);
}

int MovingAverage::simpleMovingAverage(int input, uint8_t window_size) {
  if (!this->enabled) return NULL;

  this->input = input;
  window_size = constrain(window_size, 2, 100);

  static bool filled;
  static uint8_t index;
  static int buffer[100];
  static int sum;

  sum += input;

  if (!filled) {
    buffer[index] = input;
    index++;

    if (index >= window_size) {
      filled = true;
    }
  } else {
    sum -= buffer[index];
    buffer[index] = input;
    index = (index + 1) % window_size;
  }

  this->sma_output = sum / window_size;

  return this->sma_output;
}

int MovingAverage::cumulativeAverage(int input) {
  if (!this->enabled) return NULL;

  this->input = input;

  static uint16_t index;
  static int32_t sum;

  sum += input;
  index++;

  this->ca_output = sum / index;

  return this->ca_output;
}