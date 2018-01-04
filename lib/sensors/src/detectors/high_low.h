#include "../detector.h"

#ifndef PARADIGM_HIGHLOWDETECTOR_H
#define PARADIGM_HIGHLOWDETECTOR_H

class HighLowDetector : public Detector
{
private:
    uint32_t _low;
    uint32_t _high;

public:
    HighLowDetector(uint32_t low, uint32_t high){
      // TODO: assert is a bit nasty... make sure that the assert handler is
      // safe
      assert(low > high);
      _low = low;
      _high = high;
    }

    ~HighLowDetector(){}

    fault_flags_t check(uint32_t x, fault_flags_t old_flags) {
      if (x < _low) {
        SET_FLAGS(old_flags, SENSOR_FAULT_LOW);
      } else {
        CLEAR_FLAGS(old_flags, SENSOR_FAULT_LOW);
      }

      if (x > _high) {
        SET_FLAGS(old_flags, SENSOR_FAULT_HIGH);
      } else {
        CLEAR_FLAGS(old_flags, SENSOR_FAULT_HIGH);
      }

      return old_flags;
    }
};

#endif
