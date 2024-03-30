/**
 * @file MovingAverage.h
 *
 * @brief Defines a template class for implementing various types of moving average filters.
 *
 * This file contains the declaration of the MovingAverage template class, which provides functionality
 * for calculating different types of moving averages including Simple Moving Average (SMA), Cumulative
 * Average (CA), Weighted Moving Average (WMA), and Exponential Moving Average (EMA).
 *
 * @author Maximilian Kautzsch
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
 */

#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum AverageType
{
  SMA = 1 << 0,
  CA = 1 << 1,
  WMA = 1 << 2,
  EMA = 1 << 3
};

/**
 * @brief Template class for implementing various types of moving average filters.
 *
 * This template class provides functionality for calculating different types of moving averages
 * including Simple Moving Average (SMA), Cumulative Average (CA), Weighted Moving Average (WMA),
 * and Exponential Moving Average (EMA).
 *
 * @tparam T The type of input data.
 * @tparam U The type of average calculated.
 */
template <typename T, typename U>
class MovingAverage
{
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

private:
  bool enabled;
  T input;
  U simple_moving_average;
  U cumulative_average;
  U weighted_moving_average;
  U exponential_moving_average;
};

/**
 * @brief Constructs a new MovingAverage object.
 */
template <typename T, typename U>
MovingAverage<T, U>::MovingAverage() {}

/**
 * @brief Destructs a constructed MovingAverage object.
 */
template <typename T, typename U>
MovingAverage<T, U>::~MovingAverage() {}

/**
 * @brief Initializes the MovingAverage object.
 *
 * Toggles the 'enabled' class attribtute to true.
 */
template <typename T, typename U>
void MovingAverage<T, U>::begin()
{
  this->enabled = true;
}

/**
 * @brief Stops the MovingAverage object.
 *
 * Toggles the 'enabled' class attribtute to false.
 */
template <typename T, typename U>
void MovingAverage<T, U>::end()
{
  this->enabled = false;
}

/**
 * @brief Adds a new data point to the filter.
 *
 * @param input The new data point that is being added to the filter.
 */
template <typename T, typename U>
void MovingAverage<T, U>::add(T input)
{
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
template <typename T, typename U>
void MovingAverage<T, U>::print(uint8_t average_types)
{
  while (!Serial)
  {
  }

  Serial.print("Raw-Data:");
  Serial.print(this->input);

  if (average_types & SMA)
  {
    Serial.print("\tSMA:");
    Serial.print(this->simple_moving_average);
  }
  if (average_types & CA)
  {
    Serial.print("\tCA:");
    Serial.print(this->cumulative_average);
  }
  if (average_types & WMA)
  {
    Serial.print("\tWMA:");
    Serial.print(this->weighted_moving_average);
  }
  if (average_types & EMA)
  {
    Serial.print("\tEMA:");
    Serial.print(this->exponential_moving_average);
  }

  Serial.print("\n");
}

/**
 * @brief Prints all average filter outputs.
 *
 * The raw data points as well as all of the calculated average filter outputs
 * are printed through the serial monitor.
 */
template <typename T, typename U>
void MovingAverage<T, U>::print()
{
  this->print(SMA | CA | WMA | EMA);
}

/**
 * @brief Filters out peaks from the data points.
 *
 * If the given input is greater or equal to a certain threshold value for a specified
 * amount in a row, then a data peak is retrieved.
 *
 * @param threshold The value the input data is compared to.
 * @param consecutive_matches The amount the data point must be greater or equal to the threshold
 * in a row, in order to filter out a peak.
 * @return True if a data peak has been detected, false otherwise.
 */
template <typename T, typename U>
bool MovingAverage<T, U>::detectedPeak(T threshold, uint8_t consecutive_matches)
{
  if (!this->enabled)
    return 0;

  static uint8_t matches;

  if (this->input >= threshold)
  {
    matches++;

    if (matches >= consecutive_matches)
    {
      matches = 0;
      return true;
    }
  }
  else
  {
    matches = 0;
    return false;
  }
}

/**
 * @brief Calculates the Simple Moving Average (SMA) for a given input.
 *
 * Calculates the SMA based on the provided input and window size.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param window_size The size of the data window (amount of data points used for
 * one calculation).
 * @return The calculated Simple Moving Average (SMA).
 */
template <typename T, typename U>
U MovingAverage<T, U>::readAverage(uint8_t window_size)
{
  if (!this->enabled)
    return 0;

  static uint8_t num_elements;
  static U *window = new U[window_size];
  static U sum;

  if (num_elements < window_size)
  {
    window[num_elements] = this->input;
    sum += this->input;
    num_elements++;
  }
  else
  {
    for (uint8_t i = 0; i < window_size - 1; i++)
    {
      sum -= window[i];
      window[i] = window[i + 1];
      sum += window[i];
    }
    sum -= window[window_size - 1];
    window[window_size - 1] = this->input;
    sum += window[window_size - 1];
  }

  this->simple_moving_average = sum / num_elements;

  return this->simple_moving_average;
}

/**
 * @brief Calculates the Cumulative Average (CA) for a given input.
 *
 * Calculates the CA based on the provided input.
 * Uses all of the data points up to the current datum.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @return The calculated Cumulative Average (CA).
 */
template <typename T, typename U>
U MovingAverage<T, U>::readCumulativeAverage()
{
  if (!this->enabled)
    return 0;

  static uint16_t num_elements;
  static float average;

  if (num_elements <= 2)
  {
    average += this->input;
    num_elements++;
    average /= num_elements;
  }
  else
  {
    num_elements++;
    average = (this->input + average * (num_elements - 1)) / num_elements;
  }

  this->cumulative_average = U(average);
  return this->cumulative_average;
}

/**
 * @brief Calculates the Weighted Moving Average (WMA) for a given input.
 *
 * Calculates the WMA based on the provided input and window size.
 * Gives more weight_coefficient to recent values and lesser weight_coefficient to older values.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param window_size The size of the data window (amount of data points used for
 * one calculation).
 * @return The calculated Weighted Moving Average (WMA).
 */
template <typename T, typename U>
U MovingAverage<T, U>::readWeightedAverage(uint8_t window_size)
{
  if (!this->enabled)
    return 0;

  static uint8_t num_elements;
  static U *window = new U[window_size];
  static U sum;
  uint8_t weight_coefficient;

  if (num_elements < window_size)
  {
    weight_coefficient = num_elements + 1;
    window[num_elements] = this->input;
    sum += this->input * weight_coefficient;
    num_elements++;
  }
  else
  {
    for (uint8_t i = 0; i < window_size - 1; i++)
    {
      weight_coefficient = i + 1;
      sum -= window[i] * weight_coefficient;
      window[i] = window[i + 1];
      sum += window[i] * weight_coefficient;
    }
    weight_coefficient = window_size;
    sum -= window[window_size - 1] * weight_coefficient;
    window[window_size - 1] = this->input;
    sum += this->input * weight_coefficient;
  }

  this->weighted_moving_average = 2 * sum / (num_elements * (num_elements + 1));

  return this->weighted_moving_average;
}

/**
 * @brief Calculates the Exponential Moving Average (EMA) for a given input.
 *
 * Calculates the EMA based on the provided input and smoothing factor.
 * Apply different weights to current values and the previous average.
 * If the MovingAverage object is disabled, returns 0.
 *
 * @param smoothing_factor In interval of [0; 1]. Applies more weight_coefficient to current
 * values, if > 0, or weighs previous average heavier, if < 0.
 * @return The calculated Exponential Moving Average (EMA).
 */
template <typename T, typename U>
U MovingAverage<T, U>::readExponentialAverage(float smoothing_factor)
{
  if (!this->enabled)
    return 0;

  static U average;

  this->exponential_moving_average = smoothing_factor * (this->input) + (1 - smoothing_factor) * average;
  average = this->exponential_moving_average;

  return this->exponential_moving_average;
}

#endif // MOVINGAVERAGE_H