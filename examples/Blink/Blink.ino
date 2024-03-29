/*
MAX7313 Port Expander blinky example

@author: simon

Copyright (c) 2019 eta systems GmbH. MIT License

*/

#include <Wire.h>
#include "MAX7313.h"

const int16_t ADDR_EXPANDER  = 0x24;

MAX7313 lamps(ADDR_EXPANDER); // create MAX7313 instance with corresponding I2C address

void setup() {
  Wire.begin();               // initialize I2C interface
  lamps.begin();              // start the port expander library
  lamps.pinMode(0, OUTPUT);   // declare port 0 as OUTPUT
}

void loop() {
  lamps.digitalWrite(0, LOW);    // turn off port 0
  delay(500);
  lamps.digitalWrite(0, HIGH);   // turn on port 0
  delay(500);

}