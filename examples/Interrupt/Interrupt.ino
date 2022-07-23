/*
MAX7313 Port Expander GPIO interrupt example

@author: simon

Copyright (c) 2019 eta systems GmbH. MIT License

*/

#include <Wire.h>
#include "MAX7313.h"

const int16_t ADDR_EXPANDER  = 0x27;
const byte IRQ_PIN = 2;  // connect this pin to MAX7313 INT/O16 pin

MAX7313 my_gpio(ADDR_EXPANDER); // create MAX7313 instance with corresponding I2C address

void setup() {
  Wire.begin();                 // initialize I2C interface
  my_gpio.begin();              // start the port expander library
  my_gpio.pinMode(0, INPUT);    // declare port 0 as INPUT
  
  pinMode(IRQ_PIN, INPUT_PULLUP); // signal is active LOW --> pull up enabled
  my_gpio.enableInterrupt();    // enable data change interrupt
  my_gpio.clearInterrupt();     // clear any pending interrupts
  // attach ISR function to FALLING edge GPIO interrupt on IRQ pin:
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), GPIO_ISR, FALLING);
}

void loop() {
  delay(500);
}

void GPIO_ISR(){
  my_gpio.clearInterrupt();     // acknowledge/clear interrupt so that it is armed again
  
  // do something, e.g. read the pin state
  uint8_t state = digitalRead(0);
}
