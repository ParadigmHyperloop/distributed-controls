#include "tests.h"

void test_high_low_detector() {
  // Setup detector with low watermark of 8 counts and high of 4086 counts
  HighLowDetector d1(8, 4086);

  // appropriate flags get set
  TEST_ASSERT_EQUAL(d1.check(0, 0x0), SENSOR_FAULT_LOW);
  TEST_ASSERT_EQUAL(d1.check(4095, 0x0), SENSOR_FAULT_HIGH);

  // Test that a clean flags and clean reading results in clean flags
  TEST_ASSERT_EQUAL(d1.check(2000, 0x0), 0x0);

  // Test that it adds fault bits and leaves other fault bits alone
  TEST_ASSERT_EQUAL(d1.check(0, 0x10), 0x10 | SENSOR_FAULT_LOW);
  TEST_ASSERT_EQUAL(d1.check(4095, 0x10), 0x10 | SENSOR_FAULT_HIGH);

  // Test that it clears the bits
  TEST_ASSERT_EQUAL(d1.check(2000, SENSOR_FAULT_LOW), 0x0);
  TEST_ASSERT_EQUAL(d1.check(2000, SENSOR_FAULT_HIGH), 0x0);

  // Test that it leaves other fault bits alone
  TEST_ASSERT_EQUAL(d1.check(2000, 0x10), 0x10);
}
