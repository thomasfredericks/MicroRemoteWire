// FILE main.cpp
#include <Arduino.h>
#include <Wire.h>
#include <MicroRemoteWireController.h>

constexpr uint8_t PERIPHERAL_I2C_ADDR = 0x42;

MicroRemoteWireController remote{Wire, PERIPHERAL_I2C_ADDR};

#include <MicroOscSlip.h>
MicroOscSlip<128> osc(Serial);


void setup()
{
  delay(1000); // Wait for peripherals to power up   
  Wire.begin(); // Begin as controller

  delay(1000); // Short delay to ensure Wire is ready
  // Setup example pins
  remote.setPinMode(13, OUTPUT);
  remote.digitalWrite(13, HIGH);
  
  Serial.begin(115200);
}

void loop()
{
  uint8_t state = remote.digitalRead(13);
  uint16_t analogRead0 = remote.analogRead(A0);
  osc.sendInt("/p13", state);
  osc.sendInt("/a0", analogRead0);
}
