/**
 * @file MovingAverage.h
 * @brief Template class for computing various types of moving averages.
 * @details This header provides a MovingAverage class template that enables the calculation of several
 *          types of moving averages, including Simple Moving Average (SMA), Cumulative Average (CA),
 *          Weighted Moving Average (WMA), Exponential Moving Average (EMA), and Moving Median (MM).
 *          The class supports adding new data points, printing averages, and detecting peaks, designed
 *          specifically for use in Arduino projects.
 * @author Maximilian Kautzsch
 * @date June 12, 2025
 * @copyright Copyright (c) 2024 Maximilian Kautzsch
 * Licensed under MIT License.
 */

#pragma once

#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <stdint.h>
#include "Vector.h"  // Custom vector library
#include "SkipList.h" // Updated to use custom Vector

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * @brief Enumeration for specifying the type of average calculation.
 * @details Defines different averaging methods used to determine which moving average types are calculated and printed.
 */
typedef enum
{
    SMA = 1 << 0, ///< Simple Moving Average
    CA = 1 << 1,  ///< Cumulative Average
    WMA = 1 << 2, ///< Weighted Moving Average
    EMA = 1 << 3, ///< Exponential Moving Average
    MM = 1 << 4   ///< Moving Median
} AverageType;

/**
 * @class MovingAverage
 * @brief Template class for calculating moving averages.
 * @tparam T The data type for input values (default: int16_t).
 * @tparam U The data type for average values (default: int16_t).
 * @details Provides methods to calculate and manage various types of moving averages, including adding data points,
 *          computing averages, printing results, and detecting peaks in the data.
 */
template <typename T = int16_t, typename U = int16_t>
class MovingAverage
{
public:
    /**
     * @brief Constructs a new MovingAverage object.
     * @details Initializes the MovingAverage object with default values for its attributes.
     */
    MovingAverage();

    /**
     * @brief Destructs a MovingAverage object.
     * @details Cleans up resources used by the MovingAverage object, including clearing internal vectors.
     */
    ~MovingAverage();

    /**
     * @brief Enables the MovingAverage object.
     * @details Sets the enabled flag to true, allowing the object to process data.
     */
    void begin();

    /**
     * @brief Disables the MovingAverage object.
     * @details Sets the enabled flag to false, stopping data processing.
     */
    void end();

    /**
     * @brief Adds a new data point to the moving average calculation.
     * @param input The new data point to be added.
     * @details Adds the input value to the internal data structures and marks the window as needing an update.
     */
    void add(T input);

    /**
     * @brief Prints the specified types of averages.
     * @param average_types Bitmask specifying the types of averages to print.
     * @details Outputs the raw data and the calculated averages of the specified types to the serial monitor.
     */
    void print(uint8_t average_types);

    /**
     * @brief Prints all available averages.
     * @details Outputs the raw data and all calculated averages to the serial monitor.
     */
    void print();

    /**
     * @brief Detects peaks in the data points.
     * @param threshold The threshold value for peak detection.
     * @param consecutive_matches The number of consecutive times the input must exceed the threshold to detect a peak.
     * @return True if a peak is detected, false otherwise.
     * @details Checks if the input value exceeds the threshold for the specified number of consecutive times.
     */
    bool detectedPeak(T threshold, uint8_t consecutive_matches);

    /**
     * @brief Calculates the Simple Moving Average (SMA).
     * @param window_size The size of the window for the SMA calculation.
     * @return The computed Simple Moving Average.
     * @details Computes the SMA for the given window size. Returns 0 if the object is disabled.
     */
    U readAverage(uint8_t window_size);

    /**
     * @brief Calculates the Cumulative Average (CA).
     * @return The computed Cumulative Average.
     * @details Computes the CA using all data points up to the current point. Returns 0 if the object is disabled.
     */
    U readCumulativeAverage();

    /**
     * @brief Calculates the Weighted Moving Average (WMA).
     * @param window_size The size of the window for the WMA calculation.
     * @return The computed Weighted Moving Average.
     * @details Computes the WMA for the given window size, giving more weight to recent values. Returns 0 if the object is disabled.
     */
    U readWeightedAverage(uint8_t window_size);

    /**
     * @brief Calculates the Exponential Moving Average (EMA).
     * @param smoothing_factor The smoothing factor for the EMA calculation.
     * @return The computed Exponential Moving Average.
     * @details Computes the EMA using the given smoothing factor. Returns 0 if the object is disabled.
     */
    U readExponentialAverage(float smoothing_factor);

    /**
     * @brief Calculates the Moving Median (MM).
     * @param window_size The size of the window for the MM calculation.
     * @return The computed Moving Median.
     * @details Computes the MM for the given window size using a skip list. Returns 0 if the object is disabled.
     */
    U readMovingMedian(uint8_t window_size);

private:
    /**
     * @brief Flag indicating whether the moving average is enabled.
     */
    bool enabled;

    /**
     * @brief Flag indicating whether the window has been updated since the last calculation.
     */
    bool window_updated;

    /**
     * @brief Flag indicating whether the Simple Moving Average has been calculated.
     */
    bool simple_moving_average_calculated;

    /**
     * @brief Flag indicating whether the Cumulative Average has been calculated.
     */
    bool cumulative_average_calculated;

    /**
     * @brief Flag indicating whether the Weighted Moving Average has been calculated.
     */
    bool weighted_moving_average_calculated;

    /**
     * @brief Flag indicating whether the Exponential Moving Average has been calculated.
     */
    bool exponential_moving_average_calculated;

    /**
     * @brief Flag indicating whether the Moving Median has been calculated.
     */
    bool moving_median_calculated;

    /**
     * @brief The latest input value added to the moving average.
     */
    T input;

    /**
     * @brief The computed Simple Moving Average.
     */
    U simple_moving_average;

    /**
     * @brief The computed Cumulative Average.
     */
    U cumulative_average;

    /**
     * @brief The computed Weighted Moving Average.
     */
    U weighted_moving_average;

    /**
     * @brief The computed Exponential Moving Average.
     */
    U exponential_moving_average;

    /**
     * @brief The computed Moving Median.
     */
    U moving_median;

    /**
     * @brief The vector storing the last window_size input values.
     */
    Vector<U> window;

    /**
     * @brief The vector storing all input values since the beginning.
     */
    Vector<U> cumulative_data;

    /**
     * @brief Updates the window with the current input.
     * @param window_size The size of the window.
     * @details Manages the window size by adding the current input and removing the oldest value if necessary.
     */
    void updateWindow(uint8_t window_size);

    /**
     * @brief Implementation of MovingAverage constructor.
     * @details Initializes all member variables to their default states.
     */
    MovingAverage<T, U>::MovingAverage()
        : enabled(false), window_updated(false), simple_moving_average_calculated(false),
          cumulative_average_calculated(false), weighted_moving_average_calculated(false),
          exponential_moving_average_calculated(false), moving_median_calculated(false),
          input(0), simple_moving_average(0), cumulative_average(0), weighted_moving_average(0),
          exponential_moving_average(0), moving_median(0) {}

    /**
     * @brief Implementation of MovingAverage destructor.
     * @details Clears internal vectors and resets the exponential moving average state.
     */
    MovingAverage<T, U>::~MovingAverage()
    {
        this->enabled = false;
        this->window.clear();
        this->cumulative_data.clear();
        this->exponential_moving_average = 0;
        this->exponential_moving_average_calculated = false;
    }

    /**
     * @brief Implementation of begin method.
     * @details Enables the MovingAverage object for data processing.
     */
    void MovingAverage<T, U>::begin()
    {
        this->enabled = true;
    }

    /**
     * @brief Implementation of end method.
     * @details Disables the MovingAverage object, stopping data processing.
     */
    void MovingAverage<T, U>::end()
    {
        this->enabled = false;
    }

    /**
     * @brief Implementation of add method.
     * @param input The new data point to be added.
     * @details Adds the input value to the cumulative data and marks the window for updating.
     */
    void MovingAverage<T, U>::add(T input)
    {
        this->input = input;
        this->cumulative_data.push_back(static_cast<U>(input));
        this->window_updated = false;
    }

    /**
     * @brief Implementation of print method with specified average types.
     * @param average_types Bitmask specifying the types of averages to print.
     * @details Outputs the raw data and selected averages to the serial monitor if calculated.
     */
    void MovingAverage<T, U>::print(uint8_t average_types)
    {
        while (!Serial)
        {
        }

        Serial.print("Raw-Data:");
        Serial.print(this->input);

        if (average_types & SMA && this->simple_moving_average_calculated)
        {
            Serial.print("\tSMA:");
            Serial.print(this->simple_moving_average);
        }
        if (average_types & CA && this->cumulative_average_calculated)
        {
            Serial.print("\tCA:");
            Serial.print(this->cumulative_average);
        }
        if (average_types & WMA && this->weighted_moving_average_calculated)
        {
            Serial.print("\tWMA:");
            Serial.print(this->weighted_moving_average);
        }
        if (average_types & EMA && this->exponential_moving_average_calculated)
        {
            Serial.print("\tEMA:");
            Serial.print(this->exponential_moving_average);
        }
        if (average_types & MM && this->moving_median_calculated)
        {
            Serial.print("\tMM:");
            Serial.print(this->moving_median);
        }

        Serial.print("\n");
    }

    /**
     * @brief Implementation of print method for all averages.
     * @details Outputs the raw data and all calculated averages to the serial monitor.
     */
    void MovingAverage<T, U>::print()
    {
        this->print(SMA | CA | WMA | EMA | MM);
    }

    /**
     * @brief Implementation of detectedPeak method.
     * @param threshold The threshold value for peak detection.
     * @param consecutive_matches The number of consecutive times the input must exceed the threshold.
     * @return True if a peak is detected, false otherwise.
     * @details Checks for consecutive inputs exceeding the threshold to identify peaks.
     */
    bool MovingAverage<T, U>::detectedPeak(T threshold, uint8_t consecutive_matches)
    {
        if (!this->enabled)
            return false;

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
     * @brief Implementation of readAverage method.
     * @param window_size The size of the window for the SMA calculation.
     * @return The computed Simple Moving Average.
     * @details Computes the average of the values in the window. Returns 0 if disabled.
     */
    U MovingAverage<T, U>::readAverage(uint8_t window_size)
    {
        if (!this->enabled)
            return 0;

        if (!this->window_updated)
        {
            updateWindow(window_size);
        }

        int32_t sum = 0;
        for (const auto& val : this->window)
        {
            sum += static_cast<int32_t>(val);
        }
        this->simple_moving_average = static_cast<U>(sum / this->window.size());
        this->window_updated = true;
        this->simple_moving_average_calculated = true;

        return this->simple_moving_average;
    }

    /**
     * @brief Implementation of readCumulativeAverage method.
     * @return The computed Cumulative Average.
     * @details Computes the average of all data points. Returns 0 if disabled.
     */
    U MovingAverage<T, U>::readCumulativeAverage()
    {
        if (!this->enabled)
            return 0;

        long sum = 0;
        for (const auto& val : this->cumulative_data)
        {
            sum += static_cast<long>(val);
        }
        this->cumulative_average = static_cast<U>(sum / this->cumulative_data.size());
        this->cumulative_average_calculated = true;

        return this->cumulative_average;
    }

    /**
     * @brief Implementation of readWeightedAverage method.
     * @param window_size The size of the window for the WMA calculation.
     * @return The computed Weighted Moving Average.
     * @details Computes the weighted average of the window, with more weight on recent values. Returns 0 if disabled.
     */
    U MovingAverage<T, U>::readWeightedAverage(uint8_t window_size)
    {
        if (!this->enabled)
            return 0;

        if (!this->window_updated)
        {
            updateWindow(window_size);
        }

        long weighted_sum = 0;
        long weight_total = 0;
        for (uint8_t i = 0; i < this->window.size(); i++)
        {
            uint8_t weight = i + 1;
            weighted_sum += static_cast<long>(this->window[i]) * weight;
            weight_total += weight;
        }

        this->weighted_moving_average = static_cast<U>(weighted_sum / weight_total);
        this->window_updated = true;
        this->weighted_moving_average_calculated = true;

        return this->weighted_moving_average;
    }

    /**
     * @brief Implementation of readExponentialAverage method.
     * @param smoothing_factor The smoothing factor for the EMA calculation.
     * @return The computed Exponential Moving Average.
     * @details Computes the EMA using the smoothing factor and the previous EMA value. Returns 0 if disabled.
     */
    U MovingAverage<T, U>::readExponentialAverage(float smoothing_factor)
    {
        if (!this->enabled)
            return 0;

        this->exponential_moving_average = smoothing_factor * static_cast<float>(this->input) + (1 - smoothing_factor) * static_cast<float>(this->exponential_moving_average);
        this->exponential_moving_average_calculated = true;

        return this->exponential_moving_average;
    }

    /**
     * @brief Implementation of readMovingMedian method.
     * @param window_size The size of the window for the MM calculation.
     * @return The computed Moving Median.
     * @details Computes the median of the window using a skip list. Returns 0 if disabled.
     */
    U MovingAverage<T, U>::readMovingMedian(uint8_t window_size)
    {
        if (!this->enabled)
            return 0;

        if (!this->window_updated)
        {
            updateWindow(window_size);
        }

        SkipList<U> skiplist(window_size);
        for (const auto& val : this->window)
        {
            skiplist.insert(val);
        }

        this->moving_median = skiplist.getMedian();
        this->window_updated = true;
        this->moving_median_calculated = true;

        return this->moving_median;
    }

    /**
     * @brief Implementation of updateWindow method.
     * @param window_size The size of the window.
     * @details Manages the window size by adding the current input and removing the oldest value if necessary.
     */
    void MovingAverage<T, U>::updateWindow(uint8_t window_size)
    {
        if (this->window.size() < window_size)
        {
            this->window.push_back(static_cast<U>(this->input));
        }
        else
        {
            this->window.erase(this->window.begin());
            this->window.push_back(static_cast<U>(this->input));
        }
        this->window_updated = true;
    }
};

#endif // MOVINGAVERAGE_H