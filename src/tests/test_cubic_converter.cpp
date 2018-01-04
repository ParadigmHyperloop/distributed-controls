#include "tests.h"

void test_cubic_converter() {
  CubicConverter c1(0,0,1,0);
  TEST_ASSERT_EQUAL(c1.convert(0.0), 0.0);
  TEST_ASSERT_EQUAL(c1.convert(0.5), 0.5);
  TEST_ASSERT_EQUAL(c1.convert(-120.0), -120.0);
  TEST_ASSERT_EQUAL(c1.convert(0.0), 0.0);
  TEST_ASSERT_EQUAL(c1.convert(888888.0), 888888.0);

  CubicConverter c2(0,0,1,5.5);
  TEST_ASSERT_EQUAL(c2.convert(0.0), 5.5);
  TEST_ASSERT_EQUAL(c2.convert(0.5), 6.0);
  TEST_ASSERT_EQUAL(c2.convert(-120.0), -114.5);
  TEST_ASSERT_EQUAL(c2.convert(0.0), 5.5);
  TEST_ASSERT_EQUAL(c2.convert(888888.0), 888893.5);

  CubicConverter c3(0.1,0.2,0.3,1.0);
  TEST_ASSERT_EQUAL(c3.convert(0.0), 1.0);
  // accrues floating point error (1.60 != 1.60)
  TEST_ASSERT_WITHIN(c3.convert(1.0), 1.60, 0.0001);
  TEST_ASSERT_WITHIN(c3.convert(-1), 0.8, 0.0001);
}
