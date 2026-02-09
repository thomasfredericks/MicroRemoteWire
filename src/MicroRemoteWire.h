#ifndef __MICRO_WIRE_REMOTE_H__
#define __MICRO_WIRE_REMOTE_H__
#include <Arduino.h>

enum class MicroRemoteWirePinModes : uint8_t
{
    MODE_NONE = 0,
    MODE_INPUT,
    MODE_INPUT_PULLUP,
    MODE_OUTPUT
};


// ---------------------------
// REGISTER NUMBERS
// ---------------------------
namespace MicroRemoteWireRegisters
{
    constexpr uint8_t SET_DIGITAL_MODE = 1;      // [1][dititalPinIndex][mode]
    constexpr uint8_t DIGITAL_WRITE    = 2;      // [2][dititalPinIndex][0|1]
    constexpr uint8_t ANALOG_WRITE     = 3;      // [3][dititalPinIndex][0..255]
    constexpr uint8_t ANALOG_ENABLE    = 4;      // [4][analogPinIndex][0|1]
}


// --------------------------
// SNAPSHOT
// --------------------------
// MAXIMUM BYTES IS 32 FOR I2C TRANSFER
// Bytes used =  1 (frame) + 1 (flags) + DIGITAL_PINS/2 + ANALOG_PINS*2
// 20 bytes for 14 digital pins (2 bytes) and 8 analog pins (16 bytes) = 24 bytes total, which is above the I2C limit. So we can only support up to 12 digital pins (6 bytes) and 8 analog pins (16 bytes) = 22 bytes total.
template <size_t DIGITAL_PINS = 14, size_t ANALOG_PINS = 8>
struct MicroRemoteWireSnapshot
{
    uint8_t frame; // optional frame counter
    uint8_t flags; // reserved
    uint8_t digital[DIGITAL_PINS];
    int16_t analog[ANALOG_PINS];
};

#endif // __MICRO_WIRE_REMOTE_H__

