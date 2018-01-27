#include <Arduino.h>
#include "../sensor_driver.h"

#ifndef PARADIGM_SENSORSIMDRIVER_H
#define PARADIGM_SENSORSIMDRIVER_H

class SensorSimDriver : public SensorDriver
{
private:
    int pin;
    float (*func)(int);
public:
    SensorSimDriver(int p, float (*f)(int)){
      pin = p;
      func = f;
    }

    ~SensorSimDriver(){}

    SensorReadResult read_once(void) {
      uint32_t reading = func(pin);
      return SensorReadResult(0x0, reading);
    }

    void setup(void) {

    }

    void teardown(void) {

    }
};

#endif
