# MovingAverage Library for Arduino

An easy to use Arduino library for moving average filtering.

## How to use the library

### Installing the library

Firstly, you need to download this repository as a `.zip` file. Next, from the Arduino IDE, go to `Sketch > Include Library > Add .zip-library`.

### Importing the library

You can import the module in our sketch by including the following statement in your code:

```c++
#include <MovingAverage.h>
```

### Instantiating objects

If you want to start using the functionalities of this library, you need to create an instance of the MovingAverage class.

```C++
MovingAverage filter;
```

## MovingAverage Methods

| Method                                           | Returned Parameters          | Description                                                        |
| ------------------------------------------------ | ---------------------------- | ------------------------------------------------------------------ |
| `begin()`                                        | -                            | Initializes the MovingAverage object.                              |
| `end()`                                          | -                            | Stops the MovingAverage object.                                    |
| `add(T input)`                                   | -                            | Adds a new data point to the filter.                               |
| `print(uint8_t average_types)`                   | -                            | Prints the selected average filter outputs.                        |
| `readAverage(uint8_t window_size)`               | `simple_moving_average`      | Calculates the Simple Moving Average (SMA) for a given input.      |
| `readCumulativeAverage()`                        | `cumulative_average`         | Calculates the Cumulative Average (CA) for a given input.          |
| `readWeightedAverage(uint8_t window_size)`       | `weighted_moving_average`    | Calculates the Weighted Moving Average (WMA) for a given input.    |
| `readExponentialAverage(float smoothing_factor)` | `exponential_moving_average` | Calculates the Exponential Moving Average (EMA) for a given input. |

## Examples

Examples can be founds in the [examples folder](examples) or if installed in Arduino IDE, by the `Menu > File > Example > MovingAverage`.
