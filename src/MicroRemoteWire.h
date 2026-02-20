#ifndef __MICRO_REMOTE_WIRE_H__
#define __MICRO_REMOTE_WIRE_H__

namespace MicroRemoteWire
{
    enum class Command : uint8_t
    {
        SET_PIN_MODE = 0x01,
        DIGITAL_WRITE = 0x02,
        ANALOG_WRITE = 0x03,
        DIGITAL_READ = 0x04,
        ANALOG_READ = 0x05
    };
    
    enum class Mode : uint8_t
    {
        MODE_INPUT = 0x00,
        MODE_OUTPUT = 0x01,
        MODE_INPUT_PULLUP = 0x02,
        MODE_INPUT_PULLDOWN = 0x03
    };

    enum class Value : uint8_t
    {
        VALUE_LOW = 0x00,
        VALUE_HIGH = 0x01
    };
    
}

#endif // __MICRO_REMOTE_WIRE_H__
