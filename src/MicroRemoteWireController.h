#ifndef __MICRO_REMOTE_WIRE_CONTROLLER_H__
#define __MICRO_REMOTE_WIRE_CONTROLLER_H__

#include "MicroRemoteWire.h"

class MicroRemoteWireController
{
private:
    TwoWire &wire;
    uint8_t i2cAddress;

    // ---- Internal helpers ----

    inline void sendCommand3(uint8_t cmd, uint8_t a, uint8_t b, bool stopBit = true)
    {
        wire.beginTransmission(i2cAddress);
        wire.write(cmd);
        wire.write(a);
        wire.write(b);
        wire.endTransmission(stopBit);
    }

    inline void sendCommand2(uint8_t cmd, uint8_t a, bool stopBit = true)
    {
        wire.beginTransmission(i2cAddress);
        wire.write(cmd);
        wire.write(a);
        wire.endTransmission(stopBit);
    }

public:
    MicroRemoteWireController(TwoWire &wire, uint8_t address)
        : wire(wire), i2cAddress(address) {}

    // ============================================================
    // Pin Mode (Explicit API)
    // ============================================================

    inline void setPinOutput(uint8_t pin)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::SET_PIN_MODE),
            pin,
            static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_OUTPUT));
    }

    inline void setPinInput(uint8_t pin)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::SET_PIN_MODE),
            pin,
            static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT));
    }

    inline void setPinInputPullup(uint8_t pin)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::SET_PIN_MODE),
            pin,
            static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT_PULLUP));
    }

    inline void setPinInputPulldown(uint8_t pin)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::SET_PIN_MODE),
            pin,
            static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT_PULLDOWN));
    }

    // ============================================================
    // Digital Write (Explicit API)
    // ============================================================

    inline void digitalWriteHigh(uint8_t pin)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::DIGITAL_WRITE),
            pin,
            static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_HIGH));
    }

    inline void digitalWriteLow(uint8_t pin)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::DIGITAL_WRITE),
            pin,
            static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_LOW));
    }

    // ============================================================
    // Analog Write
    // ============================================================

    inline void analogWrite(uint8_t pin, uint8_t value)
    {
        sendCommand3(
            static_cast<uint8_t>(MicroRemoteWire::Command::ANALOG_WRITE),
            pin,
            value);
    }

    // ============================================================
    // Digital Read
    // ============================================================

    uint8_t digitalRead(uint8_t pin)
    {
        sendCommand2(
            static_cast<uint8_t>(MicroRemoteWire::Command::DIGITAL_READ),
            pin, false);

        wire.requestFrom(i2cAddress, (uint8_t)1);

        if (wire.available()) {
            uint8_t value = wire.read();
            if (value == static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_HIGH))
                return HIGH;
            else if (value == static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_LOW))
                return LOW;
        }
        
        return 0;
    }

    // ============================================================
    // Analog Read
    // ============================================================

    uint16_t analogRead(uint8_t pin)
    {
        sendCommand2(
            static_cast<uint8_t>(MicroRemoteWire::Command::ANALOG_READ),
            pin, false);

        wire.requestFrom(i2cAddress, (uint8_t)2);

        uint16_t value = 0;

        if (wire.available() >= 2)
        {
            value = ((uint16_t)wire.read() << 8);
            value |= wire.read();
        }

        return value;
    }
};

#endif
