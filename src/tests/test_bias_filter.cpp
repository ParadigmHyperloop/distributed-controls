#include "tests.h"

void test_bias_filter() {
  BiasFilter f1(0.5);
  TEST_ASSERT_EQUAL(f1.getRiseTime(0.5), 1);
  TEST_ASSERT_EQUAL(f1.getRiseTime(0.75), 2);
  TEST_ASSERT_EQUAL(f1.getRiseTime(0.90), 4);
  TEST_ASSERT_EQUAL(f1.getRiseTime(0.99), 7);

  TEST_ASSERT_EQUAL(f1.filter(1000), 500);
  TEST_ASSERT_EQUAL(f1.filter(1000), 750);
  TEST_ASSERT_EQUAL(f1.filter(0), 375);
  TEST_ASSERT_EQUAL(f1.filter(0), 187.5);

  f1.reset();
  TEST_ASSERT_EQUAL(f1.filter(4095), 2047.5);

  BiasFilter f2(1.0);
  TEST_ASSERT_EQUAL(f2.getRiseTime(0.5), 0);
  TEST_ASSERT_EQUAL(f2.getRiseTime(0.75), 0);
  TEST_ASSERT_EQUAL(f2.getRiseTime(0.90), 0);
  TEST_ASSERT_EQUAL(f2.getRiseTime(0.99), 0);

  BiasFilter f3(0.001);
  TEST_ASSERT_EQUAL(f3.getRiseTime(0.5), 693);
  TEST_ASSERT_EQUAL(f3.getRiseTime(0.75), 1386);
  TEST_ASSERT_EQUAL(f3.getRiseTime(0.90), 2302);
  TEST_ASSERT_EQUAL(f3.getRiseTime(0.99), 4603);

  BiasFilter f4(0.25);
  TEST_ASSERT_EQUAL(f4.getRiseTime(0.5), 3);
  TEST_ASSERT_EQUAL(f4.getRiseTime(0.75), 5);
  TEST_ASSERT_EQUAL(f4.getRiseTime(0.90), 9);
  TEST_ASSERT_EQUAL(f4.getRiseTime(0.99), 17);

  TEST_ASSERT_EQUAL(f4.filter(1000), 250);
  TEST_ASSERT_EQUAL(f4.filter(1000), 437.5);
  TEST_ASSERT_EQUAL(f4.filter(1000), 578.125);

  f4.reset();
  TEST_ASSERT_EQUAL(f4.filter(100), 25);
}
