#ifndef __MICRO_REMOTE_WIRE_PERIPHERAL_H__
#define __MICRO_REMOTE_WIRE_PERIPHERAL_H__

#include "MicroRemoteWire.h"


#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN INPUT // Define if not available in older Arduino versions
#endif

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

        switch (static_cast<MicroRemoteWire::Command>(command_))
        {
        case MicroRemoteWire::Command::SET_PIN_MODE:
            if (wire.available() >= 2)
            {
                pin_ = wire.read();
                value_ = wire.read();
                switch (value_)
                {

                case static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_OUTPUT):
                    value_ = OUTPUT;
                    break;
                case static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT_PULLUP):
                    value_ = INPUT_PULLUP;
                    break;
                case static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT_PULLDOWN):
                    value_ = INPUT_PULLDOWN;
                    break;
                case static_cast<uint8_t>(MicroRemoteWire::Mode::MODE_INPUT):
                default:
                    value_ = INPUT; // Default to INPUT if unknown mode
                    break;
                }

                pinMode(pin_, value_);
            }
            break;

        case MicroRemoteWire::Command::DIGITAL_WRITE:
            if (wire.available() >= 2)
            {
                pin_ = wire.read();
                value_ = wire.read();
                if ( value_ == static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_HIGH)) digitalWrite(pin_, HIGH);
                else digitalWrite(pin_, LOW);
            }
            break;

        case MicroRemoteWire::Command::ANALOG_WRITE:
            if (wire.available() >= 2)
            {
                pin_ = wire.read();
                value_ = wire.read();
                analogWrite(pin_, value_);
            }
            break;

        case MicroRemoteWire::Command::DIGITAL_READ:
            if (wire.available() >= 1)
            {
                pin_ = wire.read();
                value_ = digitalRead(pin_); // Read immediately
                if (value_ == LOW) value_ = static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_LOW);
                else value_ = static_cast<uint8_t>(MicroRemoteWire::Value::VALUE_HIGH);
            }
            break;

        case MicroRemoteWire::Command::ANALOG_READ:
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
        switch (static_cast<MicroRemoteWire::Command>(command_))
        {
        case MicroRemoteWire::Command::DIGITAL_READ:
            wire.write(value_ & 0xFF); // 1 byte
            break;

        case MicroRemoteWire::Command::ANALOG_READ:
            wire.write((value_ >> 8) & 0xFF); // MSB
            wire.write(value_ & 0xFF);        // LSB
            break;

        default:
            wire.write(0); // Default response for unsupported reads
        }
    }
};

#endif // __MICRO_REMOTE_WIRE_PERIPHERAL_H__`