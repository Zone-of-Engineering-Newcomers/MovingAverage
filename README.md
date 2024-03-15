# Moving Average Library for Arduino

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

| Method    | Returned Parameters | Description                                 |
| --------- | ------------------- | ------------------------------------------- |
| `begin()` | -                   | Initializes the MovingAverage object.       |
| `end()`   | -                   | Stops the MovingAverage object.             |
| `print()` | -                   | Prints the selected average filter outputs. |

## Documentation

## Examples
