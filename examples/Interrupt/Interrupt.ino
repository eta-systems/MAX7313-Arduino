/*
MAX7313 Port Expander blinky example
Tested on Arduino Due (ARM) where every GPIO pin can be an interrupt input

@author: simon

Copyright (c) 2019 eta systems GmbH. MIT license
*/

#include <Wire.h>
#include "MAX7313.h"

const int16_t ADDR_EXPANDER  = 0x24;
const byte IRQ_PIN = 2;         // connect this pin to MAX7313 INT/O16 pin

MAX7313 my_gpio(ADDR_EXPANDER); // create MAX7313 instance with corresponding I2C address

void setup() {
  Serial.begin(115200);
  Serial.println("MAX7313");
      
  Wire.begin();                 // initialize I2C interface
  my_gpio.begin();              // start the port expander library
  my_gpio.pinMode(4, INPUT);    // declare port 0 as INPUT (attach button to MAX7313)
  my_gpio.pinMode(0, OUTPUT);   // declare as OUTPUT for simultaneous PWM generation
  
  pinMode(IRQ_PIN, INPUT_PULLUP); // signal is active LOW --> pull up enabled
  my_gpio.enableInterrupt();    // enable data change interrupt
  my_gpio.clearInterrupt();     // clear any pending interrupts
  // attach ISR function to FALLING edge GPIO interrupt on IRQ pin:
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), GPIO_ISR, FALLING);
}

void loop() {
  // produce a PWM sweep
  for(int i=0; i<15; i++){
    my_gpio.analogWrite(0, i);
    delay(100);
  }

}

/*
 * Interrupt Service Routine
 * called each time the button on the MAX7313 port 4 toggles
 */
void GPIO_ISR(){
  my_gpio.clearInterrupt();     // acknowledge/clear interrupt so that it is armed again
  
  // do something, e.g. read the pin state
  uint8_t state = digitalRead(0);

  // or just print that a change has been detected
  Serial.println("bonk!");
}
