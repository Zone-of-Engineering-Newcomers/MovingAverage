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
  Serial.print(this->ca_output);
  Serial.print("\n");
}

int MovingAverage::simpleMovingAverage(int input, uint8_t window_size) {
  if (!this->enabled) return 0;

  this->input = input;

  static uint8_t num_samples;
  static int* buffer = new int[window_size];
  static int sum;

  if (num_samples < window_size) {
    buffer[num_samples] = input;
    sum += input;
    num_samples++;
  } else {
    for (uint8_t i = 0; i < window_size - 1; i++) {
      sum -= buffer[i];
      buffer[i] = buffer[i + 1];
      sum += buffer[i];
    }
    sum -= buffer[window_size - 1];
    buffer[window_size - 1] = input;
    sum += buffer[window_size - 1];
  }

  this->sma_output = sum / min(num_samples, window_size);

  return this->sma_output;
}

int MovingAverage::cumulativeAverage(int input) {
  if (!this->enabled) return 0;

  static int16_t index;
  static float average;

  if (index <= 2) {
    average += input;
    index++;
    average /= index;
  } else {
    index++;
    average = (input + average * (index - 1)) / index;
  }

  this->ca_output = int(average);
  return this->ca_output;
}