/**
  * @file 		max3713.cpp
  * @author 	Simon Burkhardt  - github.com/mnemocron
  *           eta Systems GmbH - github.com/eta-systems
  * @date 		2019.09.04
  * @brief 		Object oriented C++ library for the MAX7313 port expander
  * @see 		github.com/mnemocron
  * @see 		https://datasheets.maximintegrated.com/en/ds/MAX7313.pdf
  * @see 		https://forum.arduino.cc/index.php?topic=9682.0
  * @see    https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
  * 
  * @copyright
  * (c) 2019 eta systems GmbH. 
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  * 
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  * 
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

#include "MAX7313.h"
#include <Wire.h>

MAX7313::MAX7313(uint8_t addr, TwoWire *i2c){
  _i2c = i2c;
  _i2caddr = addr;
}

void MAX7313::begin() {
  _i2c->begin();
  write8(MAX7313_PORTS_CONF_00_07,    0x00);  // pinmode: 0 = OUTPUT / 1 = INPUT
  write8(MAX7313_PORTS_CONF_08_15,    0x00);  
  write8(MAX7313_BLINK_PHASE_0_00_07, 0xff);
  write8(MAX7313_BLINK_PHASE_0_08_15, 0xff);
  write8(MAX7313_CONFIGURATION,       0x01);  // enable blink phase for PWM
  write8(MAX7313_OUT_INT_MA_16,       0xff);
}

void MAX7313::pinMode(uint8_t num, uint8_t mode){
  uint8_t mask = 16, addr = 0;
  if(num > 7){
    mask = 8;
    addr = MAX7313_PORTS_CONF_08_15;
  } else {
    addr = MAX7313_PORTS_CONF_00_07;
  }
  uint8_t reg = read8(addr);
  if(mode == OUTPUT){
    reg &= ~(1<<(num%mask));
  } else if(mode == INPUT){
    reg |= (1<<(num%mask));
  }
  write8(addr, reg);
}

void MAX7313::digitalWrite(uint8_t num, uint8_t val){
  if(val == HIGH)
    val = 15;
  else
    val = 0;
  analogWrite(num, val);
}

void MAX7313::analogWrite(uint8_t num, uint8_t val){
  if(val > 0xf)
    val = 0xf;

  // "[the LED] cannot be turned fully off using the PWM intensity control."
  // using Blink Mode to correct this issue
  // see: page 17: "Using PWM Intensity Controls with Blink Disabled"
  // see: page 17: "Using PWM Intensity Controls with Blink Enabled"
  // see: page 18: Table 10
  // see: page 20: Table 12 (Important !)
  // fliping the on-state in the phase 0 register to turn the port fully off
  uint8_t phase0 = read8(__max7313_get_phase_reg(num, 0));
  if(val == 0){
    // Datasheet Table 12.
    val = 0xF;  // turn on to max, because the inversion makes it completely off
    phase0 &= (~(1<<(num%8)));  // inverted on state
  } else {
    phase0 |= (1<<(num%8));     // regular  on state
  }
  write8(__max7313_get_phase_reg(num, 0), phase0);

  // two ports share one 8 bit register
  // this is why the registers needs to be read without changing the bits
  // of the other port
  //
  // D7 D6 D5 D4 | D3 D2 D1 D0
  // (odd)       | (even)
  // P 1,3,5 ... | P 0,2,4 ...
  uint8_t intensity = read8(__max7313_get_output_reg(num));
  if(num % 2) // odd  (1,3,5 ...)
    intensity = (intensity & 0x0F) + ((val&0x0F)<<4); // leave low bits and shift new intensity val to high bits
  else        // even (0,2,4 ...)
    intensity = (intensity & 0xF0) + (val & 0x0F);    // leave high bits
  write8(__max7313_get_output_reg(num), intensity);
}

void MAX7313::enableInterrupt(void){
  uint8_t conf = read8(MAX7313_CONFIGURATION);
  conf |= 0x08;
  write8(MAX7313_CONFIGURATION, conf);
  clearInterrupt();
}

void MAX7313::disableInterrupt(void){
  uint8_t conf = read8(MAX7313_CONFIGURATION);
  conf &= 0xF7;     // clear bit 3
  write8(MAX7313_CONFIGURATION, conf);
  clearInterrupt();
}

void MAX7313::clearInterrupt(void){
  read8(MAX7313_READ_IN_00_07);
  read8(MAX7313_READ_IN_08_15);
}

uint8_t MAX7313::digitalRead(uint8_t num){
 // 16 pins total, stored in 2 separate 8 bit registers
 // the following macro reads the correct one
 uint8_t all_regs = read8( __max7313_get_input_reg(num) );
 // convert pin num to bit position in 8 bit register (0->0, 1->1, 2->2, 3->4, 4->8 ... 8->0, 9->1, 10->2, 11->4 ...)
 num = 1 << (num % 8);
 all_regs &= num;      // mask with bit position
 return (uint8_t)(all_regs > 0); // convert and return boolean expression (if bit set)
}

uint8_t MAX7313::read8(uint8_t addr) {
  _i2c->beginTransmission(_i2caddr);
  _i2c->write(addr);
  _i2c->endTransmission();

  _i2c->requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return _i2c->read();
}

void MAX7313::write8(uint8_t addr, uint8_t d) {
  _i2c->beginTransmission(_i2caddr);
  _i2c->write(addr);
  _i2c->write(d);
  _i2c->endTransmission();
}
