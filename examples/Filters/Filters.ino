/**
 * @brief Applies different types of moving average filters to random data points.
 *
 * @author Maximilian Kautzsch
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
*/

#include "MovingAverage.h"

<<<<<<< Updated upstream
MovingAverage<uint8_t, uint8_t> filter;  // Create instance of the moving average class for filtering the data
=======
MovingAverage<> filter;  // Create instance of the moving average class for filtering the data
>>>>>>> Stashed changes

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  filter.begin();      // Initialize the filter
}

void loop() {
  uint8_t data_point = random(1, 100);  // Generate a random integer between 1 and 100
  filter.add(data_point);               // Add the data point to the filter

  // Calculate different types of average values
<<<<<<< Updated upstream
  uint8_t simple_moving_average = filter.readAverage(10);
  uint8_t cumulative_average = filter.readCumulativeAverage();
  uint8_t weighted_moving_average = filter.readWeightedAverage(10);
  uint8_t exponential_moving_average = filter.readExponentialAverage(0.30);
=======
  int simple_moving_average = filter.readAverage(10);
  int weighted_moving_average = filter.readWeightedAverage(10);
  int moving_median = filter.readMovingMedian(10);
  int exponential_moving_average = filter.readExponentialAverage(0.30);
  int cumulative_average = filter.readCumulativeAverage();
>>>>>>> Stashed changes

  filter.print();  // Print all of the computed output values
  delay(10);       // Wait 10ms between every iteration
}