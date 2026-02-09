#ifndef __MICRO_REMOTE_WIRE_H__
#define __MICRO_REMOTE_WIRE_H__

#include <Arduino.h>
#include <Wire.h>

enum class MicroRemoteWireRegisters : uint8_t
{
    SET_PIN_MODE = 0x01,
    DIGITAL_WRITE = 0x02,
    ANALOG_WRITE = 0x03,
    DIGITAL_READ = 0x04,
    ANALOG_READ = 0x05
};


#endif // __MICRO_REMOTE_WIRE_H__

// Add Controller and Peripheral if none where included yet
#if !defined(__MICRO_REMOTE_WIRE_CONTROLLER_H__) || !defined(__MICRO_REMOTE_WIRE_PERIPHERAL_H__)
#include "MicroRemoteWireController.h"
#include "MicroRemoteWirePeripheral.h"
#endif