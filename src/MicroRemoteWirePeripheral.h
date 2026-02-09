#ifndef __ARDUINO_TERMINALS_PERIPHERAL_H__
#define __ARDUINO_TERMINALS_PERIPHERAL_H__

#include <Arduino.h>
#include <Wire.h>
#include "MicroRemoteWire.h" // for MicroRemoteWireSnapshot, MicroRemoteWirePinModes

template <size_t DIGITAL_PINS = 14, size_t ANALOG_PINS = 8>
class MicroRemoteWirePeripheral
{
private:
    MicroRemoteWireSnapshot<DIGITAL_PINS, ANALOG_PINS> snapshot_;

    MicroRemoteWirePinModes digitalPinModes_[DIGITAL_PINS] = {};
    bool analogEnabled_[ANALOG_PINS] = {};

public:
    MicroRemoteWirePeripheral() {}

    // -------------------------------
    // UPDATE LOOP
    // -------------------------------
    void update()
    {
        // ---- DIGITAL INPUTS ----
        for (uint8_t i = 0; i < DIGITAL_PINS; ++i)
        {
            uint8_t val = 0;
            if (digitalPinModes_[i] == MicroRemoteWirePinModes::MODE_INPUT ||
                digitalPinModes_[i] == MicroRemoteWirePinModes::MODE_INPUT_PULLUP)
            {
                val = digitalRead(i);
            }

            if (val)
                snapshot_.digital[i / 8] |= (1 << (i % 8));
            else
                snapshot_.digital[i / 8] &= ~(1 << (i % 8));
        }

        // ---- ANALOG INPUTS ----
        for (uint8_t i = 0; i < ANALOG_PINS; ++i)
        {
            if (analogEnabled_[i])
            {
                snapshot_.analog[i] = analogRead(i);
            }
        }
    }

    // -------------------------------
    // I2C RECEIVE
    // -------------------------------
    void onReceive(TwoWire &wire, int numBytes)
    {
        if (numBytes < 1)
            return;

        uint8_t reg = wire.read();
        numBytes--;

        switch (reg)
        {
        // REGISTER 1: set digital pin mode
        case MicroRemoteWireRegisters::SET_DIGITAL_MODE:
        {
            if (numBytes < 2)
                return;

            uint8_t idx = wire.read();
            uint8_t mode = wire.read();
            if (idx >= DIGITAL_PINS)
                return;

            digitalPinModes_[idx] = static_cast<MicroRemoteWirePinModes>(mode);

            switch (digitalPinModes_[idx])
            {
            case MicroRemoteWirePinModes::MODE_NONE:
            case MicroRemoteWirePinModes::MODE_INPUT:
                pinMode(idx, INPUT);
                break;
            case MicroRemoteWirePinModes::MODE_INPUT_PULLUP:
                pinMode(idx, INPUT_PULLUP);
                break;
            case MicroRemoteWirePinModes::MODE_OUTPUT:
                pinMode(idx, OUTPUT);
                break;
            }
            break;
        }

        // REGISTER 2: digitalWrite single pin
        case MicroRemoteWireRegisters::DIGITAL_WRITE:
        {
            if (numBytes < 2)
                return;

            uint8_t idx = wire.read();
            uint8_t val = wire.read();
            if (idx >= DIGITAL_PINS)
                return;

            if (digitalPinModes_[idx] == MicroRemoteWirePinModes::MODE_OUTPUT)
                digitalWrite(idx, val);
            break;
        }

        // REGISTER 3: analogWrite PWM
        case MicroRemoteWireRegisters::ANALOG_WRITE:
        {
            if (numBytes < 2)
                return;

            uint8_t idx = wire.read();
            uint8_t val = wire.read();
            if (idx >= DIGITAL_PINS)
                return;

            analogWrite(idx, val);
            break;
        }

        // REGISTER 4: enable/disable analog pin
        case MicroRemoteWireRegisters::ANALOG_ENABLE:
        {
            if (numBytes < 2)
                return;

            uint8_t idx = wire.read();
            uint8_t en = wire.read();
            if (idx >= ANALOG_PINS)
                return;

            analogEnabled_[idx] = en != 0;
            break;
        }
        }
    }

    // -------------------------------
    // I2C REQUEST
    // -------------------------------
    void onRequest(TwoWire &wire)
    {
        wire.write(reinterpret_cast<uint8_t *>(&snapshot_),
                   sizeof(snapshot_));
    }
};

#endif
