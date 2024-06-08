/**
 * @file MovingAverage.h
 *
 * @brief Template class for computing various types of moving averages.
 *
 * This header provides a `MovingAverage` class template that enables the calculation of several
 * types of moving averages, such as Simple Moving Average (SMA), Cumulative Average (CA),
 * Weighted Moving Average (WMA), Exponential Moving Average (EMA), and Moving Median (MM).
 * The class supports adding new data points, printing averages, and detecting peaks.
 * It is designed for use in Arduino projects.
 *
 * @autor Maximilian Kautzsch
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
 */

#pragma once

#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <stdint.h>
#include <vector>
#include <numeric>
#include "SkipList.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * @brief Enumeration for specifying the type of average calculation.
 *
 * This enumeration allows the selection of different averaging methods.
 * It is used to determine which type of moving average should be calculated and printed.
 */
typedef enum {
  SMA = 1 << 0,  // Simple Moving Average
  CA = 1 << 1,   // Cumulative Average
  WMA = 1 << 2,  // Weighted Moving Average
  EMA = 1 << 3,  // Exponential Moving Average
  MM = 1 << 4    // Moving Median
} AverageType;

/**
 * @brief Template class for calculating moving averages.
 *
 * This class template provides methods to calculate and manage various types of moving averages.
 * It supports adding new data points, calculating different averages, printing results, and
 * detecting peaks in the data.
 *
 * @tparam T The data type for input values (default: int16_t).
 * @tparam U The data type for average values (default: int16_t).
 */
template<typename T = int16_t, typename U = int16_t>
class MovingAverage {
public:
  MovingAverage();
  ~MovingAverage();

  void begin();
  void end();
  void add(T input);
  void print(uint8_t average_types);
  void print();
  bool detectedPeak(T threshold, uint8_t consecutive_matches);
  U readAverage(uint8_t window_size);
  U readCumulativeAverage();
  U readWeightedAverage(uint8_t window_size);
  U readExponentialAverage(float smoothing_factor);
  U readMovingMedian(uint8_t window_size);

private:
  bool enabled;
  bool window_updated;
  bool simple_moving_average_calculated;
  bool cumulative_average_calculated;
  bool weighted_moving_average_calculated;
  bool exponential_moving_average_calculated;
  bool moving_median_calculated;
  T input;
  U simple_moving_average;
  U cumulative_average;
  U weighted_moving_average;
  U exponential_moving_average;
  U moving_median;
  std::vector<U> window;
  std::vector<U> cumulative_data;

  void updateWindow(uint8_t window_size);
};

/**
 * @brief Constructs a new MovingAverage object.
 *
 * Initializes the MovingAverage object with default values for its attributes.
 */
template<typename T, typename U>
MovingAverage<T, U>::MovingAverage()
  : enabled(false), window_updated(false), simple_moving_average(0), cumulative_average(0), weighted_moving_average(0), exponential_moving_average(0) {}

/**
 * @brief Destructs a MovingAverage object.
 *
 * Cleans up any resources used by the MovingAverage object.
 */
template<typename T, typename U>
MovingAverage<T, U>::~MovingAverage() {
  this->enabled = false;
  this->window.clear();
  this->cumulative_data.clear();
  this->exponential_moving_average = 0;
  this->exponential_moving_average_calculated = false;
}

/**
 * @brief Enables the MovingAverage object.
 *
 * Sets the enabled flag to true, allowing the object to start processing data.
 */
template<typename T, typename U>
void MovingAverage<T, U>::begin() {
  this->enabled = true;
}

/**
 * @brief Disables the MovingAverage object.
 *
 * Sets the enabled flag to false, stopping the object from processing data.
 */
template<typename T, typename U>
void MovingAverage<T, U>::end() {
  this->enabled = false;
}

/**
 * @brief Adds a new data point to the moving average calculation.
 *
 * Adds the given input value to the internal data structures, marking the window as updated.
 *
 * @param input The new data point to be added.
 */
template<typename T, typename U>
void MovingAverage<T, U>::add(T input) {
  this->input = input;
  this->cumulative_data.push_back(input);
  this->window_updated = false;
}

/**
 * @brief Prints the specified types of averages.
 *
 * Outputs the raw data and the calculated averages of the specified types to the serial monitor.
 *
 * @param average_types Bitmask representing the types of averages to print.
 */
template<typename T, typename U>
void MovingAverage<T, U>::print(uint8_t average_types) {
  while (!Serial) {
  }

  Serial.print("Raw-Data:");
  Serial.print(this->input);

  if (average_types & SMA && this->simple_moving_average_calculated) {
    Serial.print("\tSMA:");
    Serial.print(this->simple_moving_average);
  }
  if (average_types & CA && this->cumulative_average_calculated) {
    Serial.print("\tCA:");
    Serial.print(this->cumulative_average);
  }
  if (average_types & WMA && this->weighted_moving_average_calculated) {
    Serial.print("\tWMA:");
    Serial.print(this->weighted_moving_average);
  }
  if (average_types & EMA && this->exponential_moving_average_calculated) {
    Serial.print("\tEMA:");
    Serial.print(this->exponential_moving_average);
  }
  if (average_types & MM && this->moving_median_calculated) {
    Serial.print("\tMM:");
    Serial.print(this->moving_median);
  }

  Serial.print("\n");
}

/**
 * @brief Prints all available averages.
 *
 * Outputs the raw data and all calculated averages to the serial monitor.
 */
template<typename T, typename U>
void MovingAverage<T, U>::print() {
  this->print(SMA | CA | WMA | EMA | MM);
}

/**
 * @brief Detects peaks in the data points.
 *
 * Checks if the input value is above the specified threshold for a certain number of consecutive times.
 *
 * @param threshold The threshold value to detect peaks.
 * @param consecutive_matches The number of consecutive times the input must exceed the threshold to detect a peak.
 * @return True if a peak is detected, false otherwise.
 */
template<typename T, typename U>
bool MovingAverage<T, U>::detectedPeak(T threshold, uint8_t consecutive_matches) {
  if (!this->enabled)
    return 0;

  static uint8_t matches;

  if (this->input >= threshold) {
    matches++;

    if (matches >= consecutive_matches) {
      matches = 0;
      return true;
    }
  } else {
    matches = 0;
    return false;
  }
}

/**
 * @brief Calculates the Simple Moving Average (SMA).
 *
 * Computes the SMA for the given window size. If the object is disabled, returns 0.
 *
 * @param window_size The size of the window for the SMA calculation.
 * @return The computed Simple Moving Average.
 */
template<typename T, typename U>
U MovingAverage<T, U>::readAverage(uint8_t window_size) {
  if (!this->enabled)
    return 0;

  if (!this->window_updated) {
    updateWindow(window_size);
  }

  U sum = std::accumulate(this->window.begin(), this->window.end(), U(0));
  this->simple_moving_average = sum / this->window.size();
  this->window_updated = true;
  this->simple_moving_average_calculated = true;

  return this->simple_moving_average;
}

/**
 * @brief Calculates the Cumulative Average (CA).
 *
 * Computes the CA using all data points up to the current point. If the object is disabled, returns 0.
 *
 * @return The computed Cumulative Average.
 */
template<typename T, typename U>
U MovingAverage<T, U>::readCumulativeAverage() {
  if (!this->enabled)
    return 0;

  using V = int32_t;
  if (sizeof(T) > 16) {
    using V = int64_t;
  }

  V sum = std::accumulate(this->cumulative_data.begin(), this->cumulative_data.end(), U(0));
  this->cumulative_average = sum / this->cumulative_data.size();
  this->cumulative_average_calculated = true;

  return this->cumulative_average;
}

/**
 * @brief Calculates the Weighted Moving Average (WMA).
 *
 * Computes the WMA for the given window size, giving more weight to recent values. If the object is disabled, returns 0.
 *
 * @param window_size The size of the window for the WMA calculation.
 * @return The computed Weighted Moving Average.
 */
template<typename T, typename U>
U MovingAverage<T, U>::readWeightedAverage(uint8_t window_size) {
  if (!this->enabled)
    return 0;

  if (!this->window_updated) {
    updateWindow(window_size);
  }

  U weighted_sum = 0;
  U weight_total = 0;
  for (uint8_t i = 0; i < this->window.size(); i++) {
    uint8_t weight = i + 1;
    weighted_sum += this->window[i] * weight;
    weight_total += weight;
  }

  this->weighted_moving_average = weighted_sum / weight_total;
  this->window_updated = true;
  this->weighted_moving_average_calculated = true;

  return this->weighted_moving_average;
}

/**
 * @brief Calculates the Exponential Moving Average (EMA).
 *
 * Computes the EMA using the given smoothing factor. If the object is disabled, returns 0.
 *
 * @param smoothing_factor The smoothing factor for the EMA calculation.
 * @return The computed Exponential Moving Average.
 */
template<typename T, typename U>
U MovingAverage<T, U>::readExponentialAverage(float smoothing_factor) {
  if (!this->enabled)
    return 0;

  this->exponential_moving_average = smoothing_factor * (this->input) + (1 - smoothing_factor) * this->exponential_moving_average;
  this->exponential_moving_average_calculated = true;

  return this->exponential_moving_average;
}

/**
 * @brief Calculates the Moving Median (MM).
 *
 * Computes the MM for the given window size. If the object is disabled, returns 0.
 *
 * @param window_size The size of the window for the MM calculation.
 * @return The computed Moving Median.
 */
template<typename T, typename U>
U MovingAverage<T, U>::readMovingMedian(uint8_t window_size) {
  if (!this->enabled)
    return 0;

  if (!this->window_updated) {
    updateWindow(window_size);
  }

  SkipList<U> skiplist(window_size);
  for (const auto& val : this->window) {
    skiplist.insert(val);
  }

  this->moving_median = skiplist.getMedian();
  this->window_updated = true;
  this->moving_median_calculated = true;

  return this->moving_median;
}

/**
 * @brief Updates the window with the current input.
 *
 * Manages the window size and adds the current input to the window.
 *
 * @param window_size The size of the window.
 */
template<typename T, typename U>
void MovingAverage<T, U>::updateWindow(uint8_t window_size) {
  if (this->window.size() < window_size) {
    this->window.push_back(this->input);
  } else {
    this->window.erase(this->window.begin());
    this->window.push_back(this->input);
  }
  this->window_updated = true;
}

#endif  // MOVINGAVERAGE_H