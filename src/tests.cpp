#include "tests.h"

bool __TEST_SILENCE = false;
/*
 * Use this file to register unit tests. Currently the pio unit testing
 * runner is not working with my laptop and therefore I've built my own.
 * This unit tester requires that each test function be registered and leave
 * the device in the exact same state it found it in.  TEST_* macros are
 * defined in tests.h to create your test cases. Use those macros and define
 * more as needed.
 */
void test_high_low_detector(void);
void test_bias_filter(void);
void test_on_off_filter(void);
void test_cubic_converter(void);
void test_retriable(void);
void test_sensor_driver(void);
void test_internal_sensor_driver(void);
void test_sensor(void);
void test_node(void);
void test() {
  test_high_low_detector();
  test_bias_filter();
  test_on_off_filter();
  test_cubic_converter();
  test_retriable();
  test_sensor_driver();
  test_internal_sensor_driver();

  test_sensor();

  test_node();
}
