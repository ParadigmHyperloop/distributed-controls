#include "tests.h"


class TestValidatable : public Validatable {
  bool valid;
public:
  TestValidatable(bool v) {
    valid = v;
  }

  bool isValid(void) {
    return valid;
  }
};

class TestRetriable : public Retriable<TestValidatable>
{
private:
    int called = 0;
    int failures = 0;

protected:
    TestValidatable run_once(void) {
      called++;
      if (called <= failures) {
        // generate a failure
        return TestValidatable(false);
      } else {
        return TestValidatable(true);
      }
    }
public:
    TestRetriable(int f){
      failures = f;
    }

    ~TestRetriable(){}

    void reset(void) {
      called = 0;
    }
};

void test_retriable() {
  TestValidatable v1(false);
  TEST_ASSERT_EQUAL(v1.isValid(), false);

  TestValidatable v2(true);
  TEST_ASSERT_EQUAL(v2.isValid(), true);

  TestRetriable t1(0);
  TEST_ASSERT_EQUAL(t1.run().isValid(), true);

  TestRetriable t2(1);
  TEST_ASSERT_EQUAL(t2.run().isValid(), false);

  TestRetriable t3(1);
  t3.setRetries(1);
  TEST_ASSERT_EQUAL(t3.getRetries(), 1);
  TEST_ASSERT_EQUAL(t3.run().isValid(), true);

  TestRetriable t4(2);
  t4.setRetries(1);
  TEST_ASSERT_EQUAL(t4.getRetries(), 1);
  TEST_ASSERT_EQUAL(t4.run().isValid(), false);
}
