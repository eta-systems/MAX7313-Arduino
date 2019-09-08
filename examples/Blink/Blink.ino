/*
Created on Wed Sep 04 11:17:15 2019

@author: simon

Copyright (c) 2019 eta systems GmbH. All rights reserved.

This Software is distributed WITHOUT ANY WARRANTY; 
without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include <Wire.h>
#include "MAX7313.h"

#define SDA_PIN 4
#define SCL_PIN 5

const int16_t ADDR_MASTER = 0x01;
const int16_t ADDR_EXPANDER  = 0x27;

MAX7313 lamps(ADDR_EXPANDER);

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN, ADDR_MASTER);  // initialize I2C interface with corresponding pins
  lamps.begin();              // start the port expander library
  lamps.pinMode(1, OUTPUT);   // declare port 1 as OUTPUT
}

void loop() {
  lamps.analogWrite(1, 0);    // turn off port 1
  delay(500);
  lamps.analogWrite(1, 15);   // turn on port 1 (15 = MAX)
  delay(500);

}
