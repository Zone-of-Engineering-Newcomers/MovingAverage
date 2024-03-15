/**
 * STORAGE
 *
 * Empty file:
 * - total: 1332 Bytes (4%)
 * - global variables: 184 Bytes (8%)
 *
 * Simple Sketch:
 * - total: 5144 Bytes (14%)
 * - global variables: 277 Bytes (13%)
*/

#include "MovingAverage.h"

MovingAverage<int, int> filter;

void setup() {
  Serial.begin(9600);
  filter.begin();
}

void loop() {
  int input_value = random(1, 100);
  int sma_value = filter.simpleMovingAverage(input_value, 10);
  int ca_value = filter.cumulativeAverage(input_value);
  int wma_value = filter.weightedMovingAverage(input_value, 10);
  int ema_value = filter.exponentialMovingAverage(input_value, 10, 0.30);

  filter.print();
  delay(100);
}