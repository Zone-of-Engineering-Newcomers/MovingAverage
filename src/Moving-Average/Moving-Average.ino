/**
 * STORAGE
 *
 * Empty file:
 * - total: 51488 Bytes (19%)
 * - global variables: 6744 Bytes (20%)
 *
 * Simple Sketch:
 * - total: 51992 Bytes (20%)
 * - global variables: 6756 Bytes (20%)
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