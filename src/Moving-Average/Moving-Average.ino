/**
 * STORAGE
 *
 * Empty file:
 * - total: 1332 Bytes (4%)
 * - global variables: 184 Bytes (8%)
 *
 * Simple Sketch:
 * - total: 4660 Bytes (14%)
 * - global variables: 269 Bytes (13%)
*/

#include "MovingAverage.h"

MovingAverage filter;

void setup() {
  Serial.begin(9600);
  filter.begin();
}

void loop() {
  int input_val = random(1, 100);
  int sma_val = filter.simpleMovingAverage(input_val, 10);
  int ca_val = filter.cumulativeAverage(input_val);

  filter.print();
  delay(100);
}