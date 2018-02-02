#include <stdint.h>

class ActuatorDriver{
public:
  ActuatorDriver(){}
  virtual ~ActuatorDriver(){}
  virtual void set(uint32_t x, int ch);
};
