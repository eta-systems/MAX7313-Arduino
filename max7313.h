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
  * (c) 2019 eta systems GmbH. All rights reserved.
  * This Software is distributed WITHOUT ANY WARRANTY; 
  * without even the implied warranty of MERCHANTABILITY 
  * or FITNESS FOR A PARTICULAR PURPOSE. 
  */

#ifndef _MAX_7313_H
#define _MAX_7313_H

#include <Arduino.h>
#include <Wire.h>

/**
  * @note 		datasheet p.13 table 2. Register Address Map
  * @see 		https://datasheets.maximintegrated.com/en/ds/MAX7313.pdf
  */
#define MAX7313_READ_IN_00_07       	0x00
#define MAX7313_READ_IN_08_15       	0x01
#define MAX7313_BLINK_PHASE_0_00_07 	0x02
#define MAX7313_BLINK_PHASE_0_08_15 	0x03
#define MAX7313_PORTS_CONF_00_07    	0x06
#define MAX7313_PORTS_CONF_08_15    	0x07
#define MAX7313_BLINK_PHASE_1_00_07 	0x0A
#define MAX7313_BLINK_PHASE_1_08_15 	0x0B
#define MAX7313_OUT_INT_MA_16       	0x0E
#define MAX7313_CONFIGURATION       	0x0F
#define MAX7313_OUT_INT_01_00       	0x10
#define MAX7313_OUT_INT_03_02       	0x11
#define MAX7313_OUT_INT_05_04       	0x12
#define MAX7313_OUT_INT_07_06       	0x13
#define MAX7313_OUT_INT_09_08       	0x14
#define MAX7313_OUT_INT_11_10       	0x15
#define MAX7313_OUT_INT_13_12       	0x16
#define MAX7313_OUT_INT_15_14       	0x17
#define MAX7313_NO_PORT    	        	0x88 		// @todo check that this address is not within the address space of MAX7313 OR add check for NO_PORT befor writing to i2c bus

#define __max7313_get_regmask(port) (1<<(port%8))
#define __max7313_get_input_reg(port) ((port < 8) ? MAX7313_READ_IN_00_07 : MAX7313_READ_IN_08_15)
#define __max7313_get_output_reg(port) __max7313_output_registers[port/2]
#define __max7313_get_phase_reg(port, phase) \
    (phase ? ( (port<8)? MAX7313_BLINK_PHASE_1_00_07:MAX7313_BLINK_PHASE_1_08_15  ) : \
     ((port<8)? MAX7313_BLINK_PHASE_0_00_07:MAX7313_BLINK_PHASE_0_08_15))

static const uint8_t __max7313_output_registers[9] = {
  MAX7313_OUT_INT_01_00,
  MAX7313_OUT_INT_03_02,
  MAX7313_OUT_INT_05_04,
  MAX7313_OUT_INT_07_06,
  MAX7313_OUT_INT_09_08,
  MAX7313_OUT_INT_11_10,
  MAX7313_OUT_INT_13_12,
  MAX7313_OUT_INT_15_14,
  MAX7313_OUT_INT_MA_16
};

/*/
 *  @brief  Class that stores state and functions for interacting with MAX7313 PWM chip
 */
class MAX7313 {
 public:
  MAX7313(uint8_t addr = 0x42, TwoWire *I2C = &Wire);
  void begin();
  void pinMode(uint8_t num, uint8_t mode);
  uint8_t digitalRead(uint8_t num);
  void analogWrite(uint8_t num, uint8_t val);
  void digitalWrite(uint8_t num, uint8_t val);
  void enableInterrupt  (void);
  void disableInterrupt (void);
  void clearInterrupt   (void);

 private:
  uint8_t _i2caddr;
  
  TwoWire *_i2c;

  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);
};

#endif
