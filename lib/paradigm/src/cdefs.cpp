#include "cdefs.h"

// handle diagnostic informations given by assertion and abort program
// execution:
void __assert(const char *__func, const char *__file, int __lineno,
              const char *__sexp) {
  // transmit diagnostic informations through SerialUSB link.
  SerialUSB.print("Assertion Failed in ");
  SerialUSB.print(__func);
  SerialUSB.print(" on ");
  SerialUSB.print(__file);
  SerialUSB.print(":");
  SerialUSB.println(__lineno, DEC);
  SerialUSB.println(__sexp);
  SerialUSB.flush();
  // abort program execution.
  abort();
}
