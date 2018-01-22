#include "../converter.h"

#ifndef PARADIGM_CUBICCONVERTER_H
#define PARADIGM_CUBICCONVERTER_H

class CubicConverter : public Converter
{
private:
    float _a;
    float _b;
    float _c;
    float _d;

public:
    CubicConverter(float a, float b, float c, float d){
      _a = a;
      _b = b;
      _c = c;
      _d = d;
    }

    ~CubicConverter(){}

    float convert(float x) {
      return (_a * x * x * x) + (_b * x * x) + (_c * x) + (_d);
    }
};

#endif
