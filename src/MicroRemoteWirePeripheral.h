#ifndef __MICRO_REMOTE_WIRE_PERIPHERAL_H__
#define __MICRO_REMOTE_WIRE_PERIPHERAL_H__

#include "MicroRemoteWire.h"

class MicroRemoteWirePeripheral
{
private:
    uint8_t command_;
    uint16_t value_; // 16-bit to hold full analogRead
    uint8_t pin_;

public:
    MicroRemoteWirePeripheral() : command_(0), value_(0), pin_(0) {}

    void onReceive(TwoWire &wire, int numBytes)
    {
        if (numBytes < 1)
            return;

        command_ = wire.read(); // First byte is command

        switch (static_cast<MicroRemoteWireRegisters>(command_))
        {
        case MicroRemoteWireRegisters::SET_PIN_MODE:
            if (wire.available() >= 2)
            {
                pin_ = wire.read();
                value_ = wire.read();
                pinMode(pin_, value_);
            }
            break;

        case MicroRemoteWireRegisters::DIGITAL_WRITE:
            if (wire.available() >= 2)
            {
                pin_ = wire.read();
                value_ = wire.read();
                digitalWrite(pin_, value_);
            }
            break;

        case MicroRemoteWireRegisters::ANALOG_WRITE:
            if (wire.available() >= 2)
            {
                pin_ = wire.read();
                value_ = wire.read();
                analogWrite(pin_, value_);
            }
            break;

        case MicroRemoteWireRegisters::DIGITAL_READ:
            if (wire.available() >= 1)
            {
                pin_ = wire.read();
                value_ = digitalRead(pin_); // Read immediately
            }
            break;

        case MicroRemoteWireRegisters::ANALOG_READ:
            if (wire.available() >= 1)
            {
                pin_ = wire.read();
                value_ = analogRead(pin_); // Full 10-bit read
            }
            break;
        }

        // Flush any remaining bytes to prevent bus issues
        while (wire.available())
            wire.read();
    }

    void onRequest(TwoWire &wire)
    {
        switch (static_cast<MicroRemoteWireRegisters>(command_))
        {
        case MicroRemoteWireRegisters::DIGITAL_READ:
            wire.write(value_ & 0xFF); // 1 byte
            break;

        case MicroRemoteWireRegisters::ANALOG_READ:
            wire.write((value_ >> 8) & 0xFF); // MSB
            wire.write(value_ & 0xFF);        // LSB
            break;

        default:
                wire.write(0); // Default response for unsupported reads
        }
    }
};

#endif // __MICRO_REMOTE_WIRE_PERIPHERAL_H__`