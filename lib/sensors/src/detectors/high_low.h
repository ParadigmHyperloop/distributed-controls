#include "../detector.h"

#ifndef PARADIGM_HIGHLOWDETECTOR_H
#define PARADIGM_HIGHLOWDETECTOR_H

class HighLowDetector : public Detector
{
private:
    int32_t _low;
    int32_t _high;

public:
    HighLowDetector(uint32_t low, uint32_t high){
      assert(low > high);
      _low = low;
      _high = high;
    }

    ~HighLowDetector(){}

    fault_flags_t check(uint32_t x) {
      fault_flags_t new_flags = 0x0;
      if (x < _low) {
        new_flags |= SENSOR_FAULT_LOW;
      }

      if (x > _high) {
        new_flags |= SENSOR_FAULT_HIGH;
      }
      return new_flags;
    }
};

#endif
