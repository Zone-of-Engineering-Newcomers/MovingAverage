/**
 * @file MovingAverage.h
 *
 * @brief Header file for implementing moving average filtering.
 *
 * This file contains the declaration of the MovingAverage class,
 * which provides functionality for several variations of a
 * moving average filter.
 *
 * @date 25th February 2024
 * @author Maximilian Kautzsch
*/

#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <stdint.h>
#include <Arduino.h>

class MovingAverage {
public:
  MovingAverage();
  ~MovingAverage();

  void begin();
  void end();
  void print();
  int MovingAverage::simpleMovingAverage(int input, uint8_t window_size);
  int cumulativeAverage(int input);
  int weightedMovingAverage(int input, uint8_t window_size);
private:
  bool enabled;
  int input;
  int sma_output;
  int ca_output;
  int wma_output;
};

#endif