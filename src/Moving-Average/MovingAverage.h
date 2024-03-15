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
  int simpleMovingAverage(int input, uint8_t window_size);
  int cumulativeAverage(int input);
  int weightedMovingAverage(int input, uint8_t window_size);
  int exponentialMovingAverage(int input, uint8_t window_size, float smoothing_factor);
private:
  bool enabled;
  int input;
  int sma_output;
  int ca_output;
  int wma_output;
  int ema_output;
};

#endif