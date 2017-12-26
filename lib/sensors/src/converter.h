#include <paradigm.h>

#ifndef PARADIGM_CONVERTER_H
#define PARADIGM_CONVERTER_H

using namespace std;

class Converter {
public:
  virtual float convert(float) = 0;
};

#endif
