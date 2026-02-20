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

| Method | Description |
| --- | --- |
| Constructor | Constructs a controller instance using a TwoWire object and the I2C address of the peripheral. Parameters: `wire` (TwoWire), `address` (uint8_t) |
| `setPinOutput(pin)` | Sets the specified pin as an output on the remote peripheral. Parameters: `pin` (uint8_t) |
| `setPinInput(pin)` | Sets the specified pin as a standard input on the remote peripheral. Parameters: `pin` (uint8_t) |
| `setPinInputPullup(pin)` | Sets the specified pin as an input with pull-up resistor on the remote peripheral. Parameters: `pin` (uint8_t) |
| `setPinInputPulldown(pin)` | Sets the specified pin as an input with pull-down resistor on the remote peripheral. Parameters: `pin` (uint8_t) |
| `digitalWriteHigh(pin)` | Writes a high digital value to the specified pin on the remote peripheral. Parameters: `pin` (uint8_t) |
| `digitalWriteLow(pin)` | Writes a low digital value to the specified pin on the remote peripheral. Parameters: `pin` (uint8_t) |
| `analogWrite(pin, value)` | Writes an analog value (PWM) to the specified pin on the remote peripheral. Parameters: `pin` (uint8_t), `value` (uint8_t) |
| `digitalRead(pin)` | Reads the digital value from the specified pin on the remote peripheral. Parameters: `pin` (uint8_t). Returns: The digital value read (uint8_t) |
| `analogRead(pin)` | Reads the analog value from the specified pin on the remote peripheral. Parameters: `pin` (uint8_t). Returns: The analog value read (uint16_t) |

### `MicroRemoteWirePeripheral`

| Method | Description |
| --- | --- |
| Constructor | Constructs a peripheral instance. |
| `onReceive(wire, numBytes)` | Handles incoming I2C data from the controller. Processes commands for setting pin modes, digital writes, analog writes, digital reads, and analog reads. Parameters: `wire` (TwoWire), `numBytes` (int) |
| `onRequest(wire)` | Handles an I2C request from the controller, returning data for digital or analog reads. Parameters: `wire` (TwoWire) |
