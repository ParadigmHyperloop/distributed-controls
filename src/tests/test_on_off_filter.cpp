#include "tests.h"

const float EPSILON = 0.001;

void test_on_off_filter() {
  OnOffFilter f1(0.7, 0.3, 0.1);
  for(int i = 0; i < 6; i++){
    TEST_ASSERT_EQUAL(f1.filter(1.0), 0);
  }
  TEST_ASSERT_EQUAL(f1.filter(1.0), 1.0);
  TEST_ASSERT_EQUAL(f1.get_state(), 1.0);
  TEST_ASSERT_WITHIN(f1.get_old_value(), 0.7, EPSILON);

  for(int i = 0; i < 3; i++){
    TEST_ASSERT_EQUAL(f1.filter(0.0), 1.0);
  }
  TEST_ASSERT_EQUAL(f1.filter(0.0), 0.0);
  TEST_ASSERT_EQUAL(f1.get_state(), 0.0);
  TEST_ASSERT_WITHIN(f1.get_old_value(), 0.3, EPSILON);

  f1.reset();
  TEST_ASSERT_EQUAL(f1.get_old_value(), 0.0);

  OnOffFilter f2(0.5, 0.5, 0.05);
  for(int i = 0; i < 9; i++){
    TEST_ASSERT_EQUAL(f2.filter(1.0), 0.0);
  }
  TEST_ASSERT_EQUAL(f2.filter(1.0), 1.0);
  TEST_ASSERT_WITHIN(f2.get_old_value(), 0.5, EPSILON);
  TEST_ASSERT_EQUAL(f2.get_state(), 1.0);

  TEST_ASSERT_EQUAL(f2.filter(0.0), 0.0);
  TEST_ASSERT_WITHIN(f2.get_old_value(), 0.45, EPSILON);

}
