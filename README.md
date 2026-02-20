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

```cpp
`MicroRemoteWireController myMicroRemoteWireController(TwoWire &wire, uint8_t address);`
```
Constructs a controller instance that communicates with a remote MicroRemoteWire peripheral over I2C. The parameters are `wire` (TwoWire) specifying the I2C bus and `address` (uint8_t) specifying the I2C address of the remote peripheral.

```cpp
`myMicroRemoteWireController.setPinOutput(uint8_t pin);`
```
Sets the specified `pin` (uint8_t) as an output on the remote peripheral.

```cpp
`myMicroRemoteWireController.setPinInput(uint8_t pin);`
```
Sets the specified `pin` (uint8_t) as a standard input on the remote peripheral.

```cpp
`myMicroRemoteWireController.setPinInputPullup(uint8_t pin);`
```
Sets the specified `pin` (uint8_t) as an input with a pull-up resistor on the remote peripheral.

```cpp
`myMicroRemoteWireController.setPinInputPulldown(uint8_t pin);`
```
Sets the specified `pin` (uint8_t) as an input with a pull-down resistor on the remote peripheral.

```cpp
`myMicroRemoteWireController.digitalWriteHigh(uint8_t pin);`
```
Writes a high digital value to the specified `pin` (uint8_t) on the remote peripheral.

```cpp
`myMicroRemoteWireController.digitalWriteLow(uint8_t pin);`
```
Writes a low digital value to the specified `pin` (uint8_t) on the remote peripheral.

```cpp
`myMicroRemoteWireController.analogWrite(uint8_t pin, uint8_t value);`
```
Writes an analog value `value` (uint8_t) using PWM to the specified `pin` (uint8_t) on the remote peripheral.

```cpp
`uint8_t value = myMicroRemoteWireController.digitalRead(uint8_t pin);`
```
Reads the digital value from the specified `pin` (uint8_t) on the remote peripheral and returns it as `value` (uint8_t).

```cpp
`uint16_t value = myMicroRemoteWireController.analogRead(uint8_t pin);`
```
Reads the analog value from the specified `pin` (uint8_t) on the remote peripheral and returns it as `value` (uint16_t).

### `MicroRemoteWirePeripheral`

```cpp
`MicroRemoteWirePeripheral myMicroRemoteWirePeripheral();`
```
Constructs a peripheral instance that responds to commands from a MicroRemoteWireController over I2C.

```cpp
`myMicroRemoteWirePeripheral.onReceive(TwoWire &wire, int numBytes);`
```
Handles incoming I2C data from a controller. Processes commands for setting pin modes, digital writes, analog writes, digital reads, and analog reads. The parameters are `wire` (TwoWire) representing the I2C bus and `numBytes` (int) representing the number of bytes received.

```cpp
`myMicroRemoteWirePeripheral.onRequest(TwoWire &wire);`
```
Handles an I2C request from the controller, returning data for digital or analog reads. The parameter is `wire` (TwoWire) representing the I2C bus.
