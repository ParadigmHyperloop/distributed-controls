#include "tests.h"

class TestSensorDriver : public SensorDriver
{
private:
    int called = 0;
    int failures = 0;
public:
    TestSensorDriver(int f){
      failures = f;
    }

    ~TestSensorDriver(){}

    SensorReadResult read_once(void) {
      called++;
      if (called <= failures) {
        // generate a failure
        return SensorReadResult(0x1, called);
      } else {
        return SensorReadResult(0x0, called);
      }
    }

    void setup(void) {

    }

    void teardown(void) {

    }

    void reset(void) {
      called = 0;
    }
};

void test_sensor_driver() {
  TestSensorDriver sd1(3);

  SensorReadResult r1 = sd1.read();
  TEST_ASSERT_EQUAL(r1.isFault(), true);
  TEST_ASSERT_EQUAL(r1.isEmpty(), false);
  TEST_ASSERT_EQUAL(r1.isValid(), false);

  sd1.reset();
  sd1.setRetries(2);
  SensorReadResult r2 = sd1.read();
  TEST_ASSERT_EQUAL(r2.isFault(), true);
  TEST_ASSERT_EQUAL(r2.isEmpty(), false);
  TEST_ASSERT_EQUAL(r2.isValid(), false);

  sd1.reset();
  sd1.setRetries(3);
  SensorReadResult r3 = sd1.read();
  TEST_ASSERT_EQUAL(r3.isFault(), false);
  TEST_ASSERT_EQUAL(r3.isEmpty(), false);
  TEST_ASSERT_EQUAL(r3.isValid(), true);
}
