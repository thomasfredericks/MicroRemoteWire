# MicroRemoteWire
A micro Arduino library that enables one Arduino microcontroller to control another Arduino microcontroller over I2C/Wire.


## Basic Peripheral Example

```cpp
// FILE main.cpp
#include <Arduino.h>
#include <MicroRemoteWirePeripheral.h>

constexpr uint8_t PERIPHERAL_I2C_ADDR = 0x42;


MicroRemoteWirePeripheral peripheral;

void onReceive(int numBytes)
{
  peripheral.onReceive(Wire, numBytes);
}

void onRequest()
{
  peripheral.onRequest(Wire);
}

void setup()
{
  Wire.begin(PERIPHERAL_I2C_ADDR);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);

}

void loop()
{


}
```

## Basic Controller Example

```cpp
#include <Arduino.h>

#include <MicroRemoteWireController.h>
#define PERIPHERAL_I2C_ADDR 0x42
MicroRemoteWireController remote(Wire, PERIPHERAL_I2C_ADDR);

void setup()
{
  // Setup blink pin
  remote.setPinOutput(13);
}


void loop()
{
  remote.digitalWriteHigh(13);
  delay(1000);
  remote.digitalWriteLow(13);
  delay(1000);
}

```


## Structs and Classes

### `MicroRemoteWireController`
Controller class for communicating with a remote MicroRemoteWire peripheral over I2C. Provides explicit APIs for pin modes, digital writes, analog writes, and reads.

#### Constructors

```cpp
MicroRemoteWireController myMicroRemoteWireController(TwoWire &wire, uint8_t address);
```
Constructs a controller instance using a TwoWire object and the I2C address of the peripheral.

- `wire`: TwoWire object representing the I2C bus (`TwoWire`)
- `address`: I2C address of the remote peripheral (`uint8_t`)

#### Method `setPinOutput(pin);`

```cpp
myMicroRemoteWireController.setPinOutput(pin);
```
Sets the specified pin as an output on the remote peripheral.

- `pin`: Pin number (`uint8_t`)

#### Method `setPinInput(pin);`

```cpp
myMicroRemoteWireController.setPinInput(pin);
```
Sets the specified pin as a standard input on the remote peripheral.

- `pin`: Pin number (`uint8_t`)

#### Method `setPinInputPullup(pin);`

```cpp
myMicroRemoteWireController.setPinInputPullup(pin);
```
Sets the specified pin as an input with pull-up resistor on the remote peripheral.

- `pin`: Pin number (`uint8_t`)

#### Method `setPinInputPulldown(pin);`

```cpp
myMicroRemoteWireController.setPinInputPulldown(pin);
```
Sets the specified pin as an input with pull-down resistor on the remote peripheral.

- `pin`: Pin number (`uint8_t`)

#### Method `digitalWriteHigh(pin);`

```cpp
myMicroRemoteWireController.digitalWriteHigh(pin);
```
Writes a high digital value to the specified pin on the remote peripheral.

- `pin`: Pin number (`uint8_t`)

#### Method `digitalWriteLow(pin);`

```cpp
myMicroRemoteWireController.digitalWriteLow(pin);
```
Writes a low digital value to the specified pin on the remote peripheral.

- `pin`: Pin number (`uint8_t`)

#### Method `analogWrite(pin, value);`

```cpp
myMicroRemoteWireController.analogWrite(pin, value);
```
Writes an analog value (PWM) to the specified pin on the remote peripheral.

- `pin`: Pin number (`uint8_t`)
- `value`: Analog value to write (0-255) (`uint8_t`)

#### Method `digitalRead(pin);`

```cpp
uint8_t value = myMicroRemoteWireController.digitalRead(pin);
```
Reads the digital value from the specified pin on the remote peripheral.

- `pin`: Pin number (`uint8_t`)
- Returns: The digital value read (`uint8_t`)

#### Method `analogRead(pin);`

```cpp
uint16_t value = myMicroRemoteWireController.analogRead(pin);
```
Reads the analog value from the specified pin on the remote peripheral.

- `pin`: Pin number (`uint8_t`)
- Returns: The analog value read (`uint16_t`)

### `MicroRemoteWirePeripheral`
Peripheral class that responds to commands from a MicroRemoteWireController over I2C. Handles pin mode configuration, digital and analog writes, and digital and analog reads.

#### Constructors

```cpp
MicroRemoteWirePeripheral myMicroRemoteWirePeripheral();
```
Constructs a peripheral instance.

#### Method `onReceive(wire, numBytes);`

```cpp
myMicroRemoteWirePeripheral.onReceive(wire, numBytes);
```
Handles incoming I2C data from the controller. Processes commands for setting pin modes, digital writes, analog writes, digital reads, and analog reads.

- `wire`: TwoWire object representing the I2C bus (`TwoWire`)
- `numBytes`: Number of bytes received in the I2C transmission (`int`)

#### Method `onRequest(wire);`

```cpp
myMicroRemoteWirePeripheral.onRequest(wire);
```
Handles an I2C request from the controller, returning data for digital or analog reads.

- `wire`: TwoWire object representing the I2C bus (`TwoWire`)
