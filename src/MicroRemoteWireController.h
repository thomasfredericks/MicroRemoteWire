
#ifndef __MICRO_REMOTE_WIRE_CONTROLLER_H__
#define __MICRO_REMOTE_WIRE_CONTROLLER_H__

#include "MicroRemoteWire.h"

class MicroRemoteWireController
{
private:
    TwoWire &wire;
    uint8_t i2cAddress;

public:
    MicroRemoteWireController(TwoWire &wire, uint8_t address)
        : wire(wire), i2cAddress(address) {}

    void setPinMode(uint8_t pin, uint8_t mode)
    {
        switch (mode)
        {
        case OUTPUT:
            mode = static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_OUTPUT);
            break;
        case INPUT_PULLUP:
            mode = static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT_PULLUP);
            break;
        case INPUT_PULLDOWN:
            mode = static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT_PULLDOWN);
            break;
        case INPUT:
        default:
            mode = static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT);
            break;
        }

        wire.beginTransmission(i2cAddress);
        wire.write(static_cast<uint8_t>(MicroRemoteWire::Command::SET_PIN_MODE));
        wire.write(pin);
        wire.write(mode);
        wire.endTransmission();
    }

    void digitalWrite(uint8_t pin, uint8_t value)
    {
        wire.beginTransmission(i2cAddress);
        wire.write(static_cast<uint8_t>(MicroRemoteWire::Command::DIGITAL_WRITE));
        wire.write(pin);
        wire.write(value);
        wire.endTransmission();
    }

    void analogWrite(uint8_t pin, uint8_t value)
    {
        wire.beginTransmission(i2cAddress);
        wire.write(static_cast<uint8_t>(MicroRemoteWire::Command::ANALOG_WRITE));
        wire.write(pin);
        wire.write(value);
        wire.endTransmission();
    }

    uint8_t digitalRead(uint8_t pin)
    {
        wire.beginTransmission(i2cAddress);
        wire.write(static_cast<uint8_t>(MicroRemoteWire::Command::DIGITAL_READ));
        wire.write(pin);
        wire.endTransmission(false);

        wire.requestFrom(i2cAddress, (uint8_t)1);
        if (wire.available())
        {
            return wire.read();
        }
        return 0;
    }

    uint16_t analogRead(uint8_t pin)
    {
        wire.beginTransmission(i2cAddress);
        wire.write(static_cast<uint8_t>(MicroRemoteWire::Command::ANALOG_READ));
        wire.write(pin);
        wire.endTransmission(false);

        wire.requestFrom(i2cAddress, (uint8_t)2); // 2 bytes for full 10-bit
        uint16_t value = 0;
        if (wire.available() >= 2)
        {
            value = ((uint16_t)wire.read() << 8); // MSB
            value |= wire.read();                 // LSB
        }
        return value;
    }
};

#endif // __MICRO_REMOTE_WIRE_CONTROLLER_H__