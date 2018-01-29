#include <stdint.h>

class ActuatorDriver{
public:
  void set(uint32_t x);
  void set(uint32_t x, int ch);
};
