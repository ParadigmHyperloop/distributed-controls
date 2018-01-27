#include <Arduino.h>
#include "../sensor_driver.h"

#ifndef PARADIGM_SENSORSIMDRIVER_H
#define PARADIGM_SENSORSIMDRIVER_H

class SensorSimDriver : public SensorDriver
{
private:
    int pin;
    uint32_t (*func)(int);
public:
    SensorSimDriver(int p, uint32_t (*f)(int)){
      pin = p;
      func = f;
    }

    ~SensorSimDriver(){}

    virtual SensorReadResult read_once(void) {
      uint32_t reading = func(pin);
      return SensorReadResult(0x0, reading);
    }

    virtual void setup(void) {

    }

    virtual void teardown(void) {

    }
};

#endif
