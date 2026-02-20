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

### MicroRemoteWireController

#### Constructor(s)

```cpp
MicroRemoteWireController myMicroRemoteWireController(wire, address);
```
Constructs a MicroRemoteWireController instance to communicate with a remote peripheral over I2C. The parameter `wire` (`TwoWire`) represents the I2C bus, and `address` (`uint8_t`) represents the I2C address of the remote device.

#### setPinOutput

```cpp
 myMicroRemoteWireController.setPinOutput(pin);
```
Sets the specified `pin` (`uint8_t`) as an output on the remote peripheral.

#### setPinInput

```cpp
 myMicroRemoteWireController.setPinInput(pin);
```
Sets the specified `pin` (`uint8_t`) as a standard input on the remote peripheral.

#### setPinInputPullup

```cpp
 myMicroRemoteWireController.setPinInputPullup(pin);
```
Sets the specified `pin` (`uint8_t`) as an input with a pull-up resistor on the remote peripheral.

#### setPinInputPulldown

```cpp
 myMicroRemoteWireController.setPinInputPulldown(pin);
```
Sets the specified `pin` (`uint8_t`) as an input with a pull-down resistor on the remote peripheral.

#### digitalWriteHigh

```cpp
 myMicroRemoteWireController.digitalWriteHigh(pin);
```
Writes a high digital value to the specified `pin` (`uint8_t`) on the remote peripheral.

#### digitalWriteLow

```cpp
 myMicroRemoteWireController.digitalWriteLow(pin);
```
Writes a low digital value to the specified `pin` (`uint8_t`) on the remote peripheral.

#### analogWrite

```cpp
 myMicroRemoteWireController.analogWrite(pin, value);
```
Writes an analog PWM `value` (`uint8_t`) to the specified `pin` (`uint8_t`) on the remote peripheral.

#### digitalRead

```cpp
 uint8_t value = myMicroRemoteWireController.digitalRead(pin);
```
Reads the digital state of the specified `pin` (`uint8_t`) on the remote peripheral and returns it as `value` (`uint8_t`).

#### analogRead

```cpp
 uint16_t value = myMicroRemoteWireController.analogRead(pin);
```
Reads the analog value of the specified `pin` (`uint8_t`) on the remote peripheral and returns it as `value` (`uint16_t`).

### MicroRemoteWirePeripheral

#### Constructor(s)

```cpp
MicroRemoteWirePeripheral myMicroRemoteWirePeripheral();
```
Constructs a MicroRemoteWirePeripheral instance that responds to I2C commands from a controller.

#### onReceive

```cpp
 myMicroRemoteWirePeripheral.onReceive(wire, numBytes);
```
Handles incoming I2C data from a controller. Processes commands for setting pin modes, digital writes, analog writes, digital reads, and analog reads. Parameters are `wire` (`TwoWire`) representing the I2C bus and `numBytes` (`int`) representing the number of bytes received.

#### onRequest

```cpp
 myMicroRemoteWirePeripheral.onRequest(wire);
```
Handles an I2C request from the controller and sends the response for digital or analog reads. Parameter `wire` (`TwoWire`) represents the I2C bus.
