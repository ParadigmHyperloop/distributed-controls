#include <stdint.h>

class Converter{
public:
  virtual ~Converter(){}
  virtual uint32_t convert(float val);
};
