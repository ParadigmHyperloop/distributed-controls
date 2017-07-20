#ifndef PARADIGM_CDEFS_H
#define PARADIGM_CDEFS_H
#include <Arduino.h>

#define __ASSERT_USE_STDERR
#include <assert.h>

/////////////////////// DEBUG /////////////////////
#ifdef DEBUG

#else
#undef assert
#define assert(x, ...)
#endif


#define APPLY_MASK(buf, mask) (buf) | (mask)
#define REMOVE_MASK(buf, mask) (buf) &= ~(mask)
#endif
