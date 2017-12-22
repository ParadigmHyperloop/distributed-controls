#ifndef PARADIGM_CDEFS_H
#define PARADIGM_CDEFS_H
#include <Arduino.h>

#define __STR__(s) #s
#define __XSTR__(s) __STR__(s)

void __assert(const char *__func, const char *__file, int __lineno,
              const char *__sexp);

/////////////////////// DEBUG /////////////////////
#ifdef DEBUG

#define assert(expr) do { \
  if (!(expr)) { \
    __assert(__FUNCTION__, __FILE__, __LINE__, __XSTR__(expr)); \
  } \
} while (0);

#else

#define assert(x, ...)

#define APPLY_MASK(buf, mask) (buf) | (mask)
#define REMOVE_MASK(buf, mask) (buf) &= ~(mask)

#endif
#endif
