#include "tests.h"

void test_on_off_filter() {
  OnOffFilter f1(0.7, 0.3, 0.1);
  for(int i = 0; i < 7; i++){
    TEST_ASSERT_EQUAL(f1.filter(1.0), 0.0);
  }
  TEST_ASSERT_EQUAL(f1.filter(1.0), 1.0);

  f1.reset();
  TEST_ASSERT_EQUAL(f1.get_old_value(), 0.0);

}
