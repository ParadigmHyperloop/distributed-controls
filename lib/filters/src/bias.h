#include "filter.h"
#include <math.h>

#ifndef PARADIGM_BIASFILTER_H
#define PARADIGM_BIASFILTER_H

class BiasFilter : public Filter
{
private:
    float old_value = 0;
    float bias = 0.0;

public:
    BiasFilter(float b){
      bias = b;
    }

    ~BiasFilter(){}

    float filter(float new_value) {
      old_value = ((1.0 - bias) * old_value) + (bias * new_value);
      return old_value;
    }

    void reset() {
      old_value = 0.0;
    }

    int32_t getRiseTime(float accuracy) {
      return ceil(log(1.0-accuracy)/log(1.0-bias));
    }
};

#endif
