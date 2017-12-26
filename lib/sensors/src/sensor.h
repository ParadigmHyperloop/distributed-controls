#include <paradigm.h>

#ifndef PARADIGM_SENSOR_H
#define PARADIGM_SENSOR_H
typedef int8_t fault_flags_t;

#include "filter.h"
#include "converter.h"
#include "detector.h"



using namespace std;

#define SENSOR_FAULT_LOW 0x1
#define SENSOR_FAULT_HIGH 0x2
#define SENSOR_FAULT_VARIANCE 0x4
#define SENSOR_FAULT_HW 0x8

class Sensor {
    float value = 0.0;
    int8_t channel = 0.0;
    fault_flags_t flags = 0;
    uint32_t samples = 0;
    float rise_accuracy = 0.99;
    Filter *filter;
    Converter *converter;
    Detector *detector;
  public:
    Sensor(int8_t ch, Converter *cv, Filter *f, Detector *d){
      channel = ch;
      converter = cv;
      filter = f;
      detector = d;
    }

    ~Sensor(){}

    void addValue (uint32_t new_value) {
      flags = detector->check(new_value);

      if (!hasFaults()) {
        float filtered = filter->filter(new_value);

        value = converter->convert(filtered);

        samples++;
      } else {
        samples = 0;
      }
    };

    float getValue (void) {
      return value;
    };

    float getChannel (void) {
      return channel;
    };

    bool isValid (void) {
      return hasRisen() && !hasFaults();
    }

    bool hasFaultLow(void) {
      return flags & SENSOR_FAULT_LOW;
    }

    bool hasFaultHigh(void) {
      return flags & SENSOR_FAULT_HIGH;
    }

    bool hasFaultVariance(void) {
      return flags & SENSOR_FAULT_VARIANCE;
    }

    bool hasFaults(void) {
      return (flags != 0x0);
    }

    bool hasFaultHW(void) {
      return flags & SENSOR_FAULT_HW;
    }

    /**
     * Returns whether the sensor has enough samples stored to have surpased
     * rise time
     * @returns true if the sensor has risen, false if it has not
     */
    bool hasRisen(void) {
      int32_t rise = filter->getRiseTime(rise_accuracy);
      return (rise < 0 || (samples > (uint32_t)rise));
    }
};

#endif
