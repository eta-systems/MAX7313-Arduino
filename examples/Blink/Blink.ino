/*
MAX7313 Port Expander blinky example

@author: simon

Copyright (c) 2019 eta systems GmbH. MIT License

*/

#include <Wire.h>
#include "MAX7313.h"

const int16_t ADDR_EXPANDER  = 0x27;

MAX7313 lamps(ADDR_EXPANDER); // create MAX7313 instance with corresponding I2C address

void setup() {
  Wire.begin();               // initialize I2C interface
  lamps.begin();              // start the port expander library
  lamps.pinMode(1, OUTPUT);   // declare port 1 as OUTPUT
}

void loop() {
  lamps.analogWrite(1, 0);    // turn off port 1
  delay(500);
  lamps.analogWrite(1, 15);   // turn on port 1 (15 = MAX)
  delay(500);

}
