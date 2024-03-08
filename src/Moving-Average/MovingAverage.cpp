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
  Serial.print("\tWMA:");
  Serial.println(this->wma_output);
}

int MovingAverage::simpleMovingAverage(int input, uint8_t window_size) {
  if (!this->enabled) return NULL;

  this->input = input;

  static bool filled;
  static uint8_t index;
  static int* buffer = new int[window_size];
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

int MovingAverage::weightedMovingAverage(int input, uint8_t window_size)
{
  if (!this->enabled) return NULL;

  this->input = input;
  window_size = constrain(window_size, 2, 100);

  static bool filled;
  static uint8_t index;
  static int buffer[100];
  static int sum;

  sum += input * (window_size - index);

  if (!filled) {
    buffer[index] = input * (window_size - index);
    index++;

    if (index >= window_size) {
      filled = true;
    }
  } else {
    sum -= buffer[index];
    buffer[index] = input * (window_size - index);
    index = (index + 1) % window_size;
  }

  this->wma_output = sum / (index * window_size - index);

  return this->wma_output;
}