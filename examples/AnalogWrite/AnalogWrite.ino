/*
MAX7313 Port Expander analogWrite (PWM) example

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
  for(int i=0; i<16; i++){
    lamps.analogWrite(0, i);
    delay(100);
  }
  for(int i=0; i<16; i++){
    lamps.analogWrite(0, 15-i);
    delay(100);
  }
}