#ifndef PARADIGM_TESTS_H
#define PARADIGM_TESTS_H

#include <Arduino.h>
#include <sensor.h>
#include "node.h"

#include <bias.h>
#include <on_off.h>
#include <converters/cubic.h>
#include <sensor_drivers/internal.h>
#include <detectors/high_low.h>

extern bool __TEST_SILENCE;

#define __TEST(cond, a, b) do { \
  if (cond) { \
    if (!__TEST_SILENCE) { \
      SerialUSB.print("[PASS] ["); \
      SerialUSB.print(__FUNCTION__); \
      SerialUSB.print("] "); \
      SerialUSB.print(__XSTR__(cond)); \
      SerialUSB.print("\n"); \
    } \
  } else { \
    SerialUSB.print("[FAIL] ["); \
    SerialUSB.print(__FUNCTION__); \
    SerialUSB.print("] "); \
    SerialUSB.print(__XSTR__(cond)); \
    SerialUSB.print(" actual lhs:"); \
    SerialUSB.print(a); \
    SerialUSB.print(" rhs:"); \
    SerialUSB.print(b); \
    SerialUSB.print("\n"); \
    abort(); \
  } \
} while (0);

// Basic test case helpers
#define TEST_ASSERT_EQUAL(a, b) __TEST((a) == (b), (a), (b))
#define TEST_ASSERT_NOTEQUAL(a, b) __TEST((a) != (b), (a), (b))
#define TEST_ASSERT_WITHIN(a, b, e) __TEST(((a) < ((b) + (e)) && (a) > ((b) - (e))), (a), (b))


// Used to prevent output of passing test cases for things like tight loops
#define TEST_SILENCE(x) do { \
  __TEST_SILENCE = true; \
  (x); \
  __TEST_SILENCE = false; \
} while (0);

/**
 * Top Level Test Function. Gets called during setup() after SerialUSB is
 * established.
 *
 * Runs all unit tests. Calls abort() should a test fail.
 */
void test(void);

#endif
