/**
 * @file MovingAverage.h
 *
 * @author Maximilian Kautzsch
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
 *
*/

#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum AverageType {
  SMA = 1 << 0,
  CA = 1 << 1,
  WMA = 1 << 2,
  EMA = 1 << 3
};

template<typename T, typename U>
class MovingAverage {
public:
  MovingAverage();
  ~MovingAverage();

  void begin();
  void end();
  void add(T input);
  void print(uint8_t average_types);
  void print();
  U readAverage(uint8_t window_size);
  U readCumulativeAverage();
  U readWeightedAverage(uint8_t window_size);
  U readExponentialAverage(float smoothing_factor);

private:
  bool enabled;
  T input;
  U sma_output;
  U ca_output;
  U wma_output;
  U ema_output;
};

/**
 * @brief Constructs a new MovingAverage object.
*/
template<typename T, typename U>
MovingAverage<T, U>::MovingAverage() {}

/**
 * @brief Destructs a constructed MovingAverage object.
*/
template<typename T, typename U>
MovingAverage<T, U>::~MovingAverage() {}

/**
 * @brief Initializes the MovingAverage object.
 *
 * Toggles the 'enabled' class attribtute to true.
*/
template<typename T, typename U>
void MovingAverage<T, U>::begin() {
  this->enabled = true;
}

/**
 * @brief Stops the MovingAverage object.
 *
 * Toggles the 'enabled' class attribtute to false.
*/
template<typename T, typename U>
void MovingAverage<T, U>::end() {
  this->enabled = false;
}

/**
 * @brief Adds a new data point to the filter.
 *
 * @param input The new data point that is being added to the filter.
*/
template<typename T, typename U>
void MovingAverage<T, U>::add(T input) {
  this->input = input;
}

/**
 * @brief Prints the selected average filter outputs.
 *
 * Firstly, the raw data points are printed serially.
 * Then, the corresponding average values are printed through the serial monitor
 * based on the selected average types.
 *
 * @param average_types Bitmask representing the average types to print.
*/
template<typename T, typename U>
void MovingAverage<T, U>::print(uint8_t average_types) {
  while (!Serial) {
  }

  Serial.print("Raw-Data:");
  Serial.print(this->input);

  if (average_types & SMA) {
    Serial.print("\tSMA:");
    Serial.print(this->sma_output);
  }
  if (average_types & CA) {
    Serial.print("\tCA:");
    Serial.print(this->ca_output);
  }
  if (average_types & WMA) {
    Serial.print("\tWMA:");
    Serial.print(this->wma_output);
  }
  if (average_types & EMA) {
    Serial.print("\tEMA:");
    Serial.print(this->ema_output);
  }

  Serial.print("\n");
}

/**
 * @brief Prints all average filter outputs.
 *
 * The raw data points as well as all of the calculated average filter outputs
 * are printed through the serial monitor.
*/
template<typename T, typename U>
void MovingAverage<T, U>::print() {
  this->print(SMA | CA | WMA | EMA);
}

/**
 * @brief Calculates the Simple Moving Average (SMA) for a given input.
 *
 * Calculates the SMA based on the provided input and window size.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param input The input data point to be processed.
 * @param window_size The size of the data window (amount of data points used for
 * one calculation).
 * @return The calculated Simple Moving Average (SMA).
*/
template<typename T, typename U>
U MovingAverage<T, U>::readAverage(uint8_t window_size) {
  if (!this->enabled) return 0;

  static uint8_t num_samples;
  static U *buffer = new U[window_size];
  static U sum;

  if (num_samples < window_size) {
    buffer[num_samples] = this->input;
    sum += this->input;
    num_samples++;
  } else {
    for (uint8_t i = 0; i < window_size - 1; i++) {
      sum -= buffer[i];
      buffer[i] = buffer[i + 1];
      sum += buffer[i];
    }
    sum -= buffer[window_size - 1];
    buffer[window_size - 1] = this->input;
    sum += buffer[window_size - 1];
  }

  this->sma_output = sum / num_samples;

  return this->sma_output;
}

/**
 * @brief Calculates the Cumulative Average (CA) for a given input.
 *
 * Calculates the CA based on the provided input.
 * Uses all of the data points up to the current datum.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param input The input data point to be processed.
 * @return The calculated Cumulative Average (CA).
*/
template<typename T, typename U>
U MovingAverage<T, U>::readCumulativeAverage() {
  if (!this->enabled) return 0;

  static uint16_t index;
  static float average;

  if (index <= 2) {
    average += this->input;
    index++;
    average /= index;
  } else {
    index++;
    average = (this->input + average * (index - 1)) / index;
  }

  this->ca_output = U(average);
  return this->ca_output;
}

/**
 * @brief Calculates the Weighted Moving Average (WMA) for a given input.
 *
 * Calculates the WMA based on the provided input and window size.
 * Gives more weight to recent values and lesser weight to older values.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param input The input data point to be processed.
 * @param window_size The size of the data window (amount of data points used for
 * one calculation).
 * @return The calculated Weighted Moving Average (WMA).
*/
template<typename T, typename U>
U MovingAverage<T, U>::readWeightedAverage(uint8_t window_size) {
  if (!this->enabled) return 0;

  static uint8_t num_samples;
  static U *buffer = new U[window_size];
  static U sum;
  uint8_t weight;

  if (num_samples < window_size) {
    weight = num_samples + 1;
    buffer[num_samples] = this->input;
    sum += this->input * weight;
    num_samples++;
  } else {
    for (uint8_t i = 0; i < window_size - 1; i++) {
      weight = i + 1;
      sum -= buffer[i] * weight;
      buffer[i] = buffer[i + 1];
      sum += buffer[i] * weight;
    }
    weight = window_size;
    sum -= buffer[window_size - 1] * weight;
    buffer[window_size - 1] = this->input;
    sum += this->input * weight;
  }

  this->wma_output = 2 * sum / (num_samples * (num_samples + 1));

  return this->wma_output;
}

/**
 * @brief Calculates the Exponential Moving Average (EMA) for a given input.
 *
 * Calculates the EMA based on the provided input and smoothing factor.
 * Apply different weights to current values and the previous average.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param input The input data point to be processed.
 * @param smoothing_factor In interval of [0; 1]. Applies more weight to current
 * values, if > 0, or weighs previous average heavier, if < 0.
 * @return The calculated Exponential Moving Average (EMA).
*/
template<typename T, typename U>
U MovingAverage<T, U>::readExponentialAverage(float smoothing_factor) {
  if (!this->enabled) return 0;
  
  static U last_average;
  
  this->ema_output = smoothing_factor * (this->input) + (1 - smoothing_factor) * last_average;
  last_average = this->ema_output;

  return this->ema_output;
}

#endif