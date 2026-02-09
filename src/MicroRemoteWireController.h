#ifndef __ARDUINO_TERMINALS_CONTROLLER_H__
#define __ARDUINO_TERMINALS_CONTROLLER_H__

#include <Arduino.h>
#include <Wire.h>
#include "MicroRemoteWire.h" // for MicroRemoteWireSnapshot, MicroRemoteWirePinModes

template <size_t DIGITAL_PINS = 14, size_t ANALOG_PINS = 8>
class ArduinoTerminalsController
{
private:
    uint8_t i2cAddress_;
    MicroRemoteWireSnapshot<DIGITAL_PINS, ANALOG_PINS> snapshot_;
    TwoWire & wire_;

public:
    explicit ArduinoTerminalsController(TwoWire &wire, uint8_t address)
        : i2cAddress_(address)
    {
    }

    // -------------------------------
    // REQUEST SNAPSHOT
    // -------------------------------
    bool update()
    {
        wire_.requestFrom(i2cAddress_, (uint8_t)sizeof(snapshot_));
        uint8_t bytesRead = 0;

        while (wire_.available() && bytesRead < sizeof(snapshot_))
        {
            ((uint8_t *)&snapshot_)[bytesRead++] = wire_.read();
        }

        return bytesRead == sizeof(snapshot_); // true if full snapshot received
    }

    // -------------------------------
    // SET PIN MODE
    // -------------------------------
    void setDigitalPinMode(uint8_t pinIndex, MicroRemoteWirePinModes mode)
    {
        wire_.beginTransmission(i2cAddress_);
        wire_.write(MicroRemoteWireRegisters::SET_DIGITAL_MODE);           // register 1
        wire_.write(pinIndex);    // pin index
        wire_.write(static_cast<uint8_t>(mode));
        wire_.endTransmission();
    }

    void enableAnalogPin(uint8_t pinIndex)
    {
        wire_.beginTransmission(i2cAddress_);
        wire_.write(MicroRemoteWireRegisters::ANALOG_ENABLE);           // register 4
        wire_.write(pinIndex);    // pin index
        wire_.write(1);
        wire_.endTransmission();
    }


    void disableAnalogPin(uint8_t pinIndex)
    {
        wire_.beginTransmission(i2cAddress_);
        wire_.write(MicroRemoteWireRegisters::ANALOG_ENABLE);           // register 4
        wire_.write(pinIndex);    // pin index
        wire_.write(0);
        wire_.endTransmission();
    }

    // -------------------------------
    // WRITE DIGITAL PIN
    // -------------------------------
    void writeDigitalPin(uint8_t pinIndex, bool val)
    {
        wire_.beginTransmission(i2cAddress_);
        wire_.write(MicroRemoteWireRegisters::DIGITAL_WRITE); // register 2
        wire_.write(pinIndex);
        wire_.write(val ? 1 : 0);
        wire_.endTransmission();
    }

    // -------------------------------
    // WRITE ANALOG PIN
    // -------------------------------
    void writeAnalogPin(uint8_t pinIndex, uint8_t val)
    {
        wire_.beginTransmission(i2cAddress_);
        wire_.write(MicroRemoteWireRegisters::ANALOG_WRITE); // register 3
        wire_.write(pinIndex);
        wire_.write(val);
        wire_.endTransmission();
    }

    // -------------------------------
    // GET DIGITAL VALUE FROM SNAPSHOT
    // -------------------------------
    bool getDigital(uint8_t pinIndex) const
    {
        if (pinIndex >= DIGITAL_PINS)
            return false;
        return (snapshot_.digital[pinIndex / 8] >> (pinIndex % 8)) & 0x01;
    }

    // -------------------------------
    // GET ANALOG VALUE FROM SNAPSHOT
    // -------------------------------
    int16_t getAnalog(uint8_t analogIndex) const
    {
        if (analogIndex >= ANALOG_PINS)
            return 0;
        return snapshot_.analog[analogIndex];
    }
};

#endif
