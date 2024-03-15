/**
 * @brief Simple example for using different types of averageing filters.
 *
 * STORAGE
 *
 * Empty file:
 * - total: 1332 Bytes (4%)
 * - global variables: 184 Bytes (8%)
 *
 * Simple Sketch:
 * - total: 5144 Bytes (15%)
 * - global variables: 277 Bytes (13%)
*/

#include "MovingAverage.h"

// Create instance of the moving average class for filtering the data
MovingAverage<int, int> filter;

void setup() {
  Serial.begin(9600);

  // Initialize the filter
  filter.begin();
}

void loop() {
  // Generate a random data point
  int data_point = random(1, 100);

  // Add the data point to the filter
  filter.add(data_point);

  // Calculate different types of average values
  int simple_moving_average = filter.readAverage(10);
  int cumulative_average = filter.readCumulativeAverage();
  int weighted_moving_average = filter.readWeightedAverage(10);
  int exponential_moving_average = filter.readExponentialAverage(0.30);

  // Print all of the average values every 10ms
  filter.print();
  delay(10);
}