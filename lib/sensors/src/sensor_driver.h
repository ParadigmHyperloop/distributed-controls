#include <paradigm.h>
#include <sensor.h>

#ifndef PARADIGM_SENSORDRIVER_H
#define PARADIGM_SENSORDRIVER_H

using namespace std;

class SensorReadResult : public Validatable {
    uint32_t reading = 0;
    fault_flags_t fault_flags = 0;
    bool reading_set = false;

public:
    SensorReadResult(fault_flags_t f, uint32_t r) {
      reading = r;
      reading_set = true;
      fault_flags = f;
    }

    SensorReadResult(fault_flags_t f) {
      fault_flags = f;
    }

    ~SensorReadResult() {}

    bool isValid() {
      return !isEmpty() && !isFault();
    };

    bool isEmpty() {
      return !reading_set;
    };

    bool isFault() {
      return (fault_flags != 0x0);
    };

    uint32_t getReading() {
      return reading;
    };

    void clearReading(void) {
      reading = 0;
      reading_set = false;
    };

    uint32_t getFlags() {
      return fault_flags;
    };
};


class SensorDriver : public Retriable<SensorReadResult> {
public:
    virtual ~SensorDriver() {}
    virtual void setup(void) = 0;
    virtual void teardown(void) = 0;
    virtual SensorReadResult read_once(void) = 0;

    SensorReadResult read(void) {
      return run();
    };

    SensorReadResult run_once(void) {
      return read_once();
    };
};

#endif
