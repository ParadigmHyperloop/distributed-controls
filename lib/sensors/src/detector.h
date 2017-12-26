#include <paradigm.h>
#include "sensor.h"

#ifndef PARADIGM_DETECTOR_H
#define PARADIGM_DETECTOR_H

using namespace std;

class Detector {
public:
  virtual fault_flags_t check(uint32_t value) = 0;
};

#endif
