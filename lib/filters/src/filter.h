#include <paradigm.h>

#ifndef PARADIGM_FILTER_H
#define PARADIGM_FILTER_H

using namespace std;

class Filter {
public:
    virtual ~Filter() {}
    virtual float filter(float n) = 0;
    virtual void reset(void) = 0;
    virtual int32_t getRiseTime(float accuracy) = 0;
};

#endif
