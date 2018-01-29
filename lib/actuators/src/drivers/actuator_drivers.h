#include <stdint.h>

class ActuatorDriver{
public:
  virtual void set(uint32_t x);
  virtual void set(uint32_t x, int ch);
};
