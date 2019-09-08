# MAX7313-Arduino
Arduino Library for the MAX7313 Port Expander by **eta systems GmbH**

---

### Usage

```cpp
#include <Wire.h>
#include "MAX7313.h"

MAX7313 pwm(0x27);  // on I2C address 0x27

void setup() {
  pwm.begin();
  pwm.pinMode(0, OUTPUT);
  pwm.pinMode(1, INPUT);
}

void loop(){
    pwm.analogWrite(0, 5);            // set PWM in range 0-15
    pwm.digitalWrite(0, HIGH);        // turn PWM to max
    uint8_t val = pwm.digitalRead(1); // read port 1
}

```

---

**MIT License**

> Copyright (c) 2019 eta systems GmbH. All rights reserved.
> 
> This Software is distributed WITHOUT ANY WARRANTY; 
> without even the implied warranty of MERCHANTABILITY 
> or FITNESS FOR A PARTICULAR PURPOSE. 




