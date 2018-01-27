#include <stdint.h>
#include <cmath>
#include <Arduino.h>

const uint32_t con = 5;

uint32_t constant(int mod){
  return con + mod;
}

uint32_t sin(){
  return static_cast<uint32_t>(2048*sin(millis()) + 2048);
}
