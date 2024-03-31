# MovingAverage library

## Methods

### `begin()`

Initializes the MovingAverage object.

#### Syntax

```C++
filter.begin();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`

### `end()`

Stops the MovingAverage object. Toggles the 'enabled' class attribute to false.

#### Syntax

```C++
filter.end();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`

### `add()`

Adds a new data point to the filter.

#### Syntax

```C++
filter.add(input);
```

#### Parameters

- _filter_: A variable type of `MovingAverage`
- _input_: The new data point that is being added to the filter

### `print()`

Prints the selected average filter outputs. Firstly, the raw data points are printed serially. Then, the corresponding average values are printed through the serial monitor based on the selected average types.

#### Syntax

```C++
filter.print();
filter.print(average_types);
```

#### Parameters

- _filter_: A variable type of `MovingAverage`
- _average_types_ (optional): Bitmask representing the average types to print

#### Example

```C++
#include <MovingAverage.h>

MovingAverage<int, int> filter;

void setup()
{
    Serial.begin(9600);
    filter.begin();
}

void loop()
{
    int data_point = random(1, 100);
    filter.add(data_point);
    filter.print();
}
```

### `detectedPeak()`

Filters out peaks from the data points. If the given input is greater or equal to a certain threshold value for a specified amount in a row, then a data peak is retrieved.

#### Syntax

```C++
filter.detectedPeak();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`
- _threshold_: The value the input data is compared to
- _consecutive_matches_: The amount the data point must be greater or equal to the threshold in a row, in order to filter out a peak

#### Example

```C++
#include <MovingAverage.h>

MovingAverage<int, int> filter;

void setup()
{
    Serial.begin(9600);
    filter.begin();
}

void loop()
{
    int data_point = random(1, 100);
    filter.add(data_point);
    bool peak = filter.detectedPeak(70, 3);
    Serial.println(peak);
}
```

#### Returns

True if a data peak has been detected, false otherwise.

### `readAverage()`

Calculates the Simple Moving Average (SMA) based on the provided input and window size. If the MovingAverage object is disabled, returns 0.

#### Syntax

```C++
filter.readAverage();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`
- _window_size_: The size of the data window (amount of data points used for one calculation)

#### Returns

The calculated Simple Moving Average (SMA).

### `readCumulativeAverage()`

Calculates the Cumulative Average (CA) based on the provided input. Uses all of the data points up to the current datum. If the MovingAverage object is disabled, returns 0.

#### Syntax

```C++
filter.readCumulativeAverage();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`

#### Returns

The calculated Cumulative Average (CA).

### `readWeightedAverage()`

Calculates the Weighted Moving Average (WMA) for a given input. Gives more weight to recent values and lesser weight to older values. If the MovingAverage object is disabled, returns 0.

#### Syntax

```C++
filter.readWeightedAverage();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`
- _window_size_: The size of the data window (amount of data points used for one calculation)

#### Returns

The calculated Weighted Moving Average (WMA).

### `readExponentialAverage()`

Calculates the Exponential Moving Average (EMA) for a given input. Apply different weights to current values and the previous average. If the MovingAverage object is disabled, returns 0.

#### Syntax

```C++
filter.readExponentialAverage();
```

#### Parameters

- _filter_: A variable type of `MovingAverage`
- _smoothing_factor_: In interval of [0; 1]. Applies more weight to current values, if > 0, or weighs previous average heavier, if < 0.

#### Returns

The calculated Exponential Moving Average (EMA).
