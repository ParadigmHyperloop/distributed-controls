#ifndef PARADIGM_H
#define PARADIGM_H

#include "cdefs.h"
#include "helpers.h"

#define output(type, ...)                                                      \
  do {                                                                         \
    SerialUSB.print("[");                                                         \
    SerialUSB.print(millis());                                                    \
    SerialUSB.print("] ");                                                        \
    SerialUSB.print("[");                                                         \
    SerialUSB.print((type));                                                      \
    SerialUSB.print("] [");                                                       \
    SerialUSB.print(__FUNCTION__);                                                \
    SerialUSB.print(" in ");                                                      \
    SerialUSB.print(__FILE__);                                                    \
    SerialUSB.print(":");                                                         \
    SerialUSB.print(__LINE__, DEC);                                               \
    SerialUSB.print("] ");                                                        \
    SerialUSB.print(__VA_ARGS__);                                                 \
    SerialUSB.print("\n");                                                        \
    SerialUSB.flush();                                                            \
  } while (0);

#ifdef DEBUG
#define debug(...) output("DEBUG", __VA_ARGS__)
#else
#define debug(...)
#endif

#define info(...) output("INFO", __VA_ARGS__)
#define warn(...) output("WARNI", __VA_ARGS__)
#define error(...) output("ERROR", __VA_ARGS__)
#define panic(...)                                                             \
  do {                                                                         \
    output("PANIC", __VA_ARGS__);                                              \
    abort();                                                                   \
  } while (0);

#define __panic_virtual_method() panic("Virtual Method Called")

// Set and clear flag(s). f is the flag(s) itself (0x4, 0x8, 0x10, etc)
#define SET_FLAGS(x,f) ((x) |= (f))
#define CLEAR_FLAGS(x,f) ((x) &= ~(f))

// Set and clear bits in a mask.  n is the index from LSB to set/clear
#define SET_BIT(x,n) ((x) |= (1 << (n)))
#define CLEAR_BIT(x,n) ((x) &= ~(1 << (n)))

#endif
