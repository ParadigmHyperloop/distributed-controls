#include <Arduino.h>
#include "../sensor_driver.h"

#ifndef PARADIGM_INTERNALSENSORDRIVER_H
#define PARADIGM_INTERNALSENSORDRIVER_H

class InternalSensorDriver : public SensorDriver
{
private:
    int pin;
public:
    InternalSensorDriver(int p){
      pin = p;
    }

    ~InternalSensorDriver(){}

    SensorReadResult read_once(void) {
      uint32_t reading = analogRead(pin);
      return SensorReadResult(0x0, reading);
    }

    void setup(void) {
      // Ensure channel is set to input
      pinMode(pin, INPUT);
      // Clear Pullup resistor
      digitalWrite(pin, LOW);
    }

    void teardown(void) {

    }
};

#endif
