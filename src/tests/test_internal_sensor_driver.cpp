#include "tests.h"

void test_internal_sensor_driver() {
  InternalSensorDriver sd1(A1);
  sd1.setup();

  TEST_ASSERT_EQUAL(sd1.getRetries(), 0);
  sd1.setRetries(1);
  TEST_ASSERT_EQUAL(sd1.getRetries(), 1);

  int i;
  for (i = 0; i < 1000; i++) {
    SensorReadResult result = sd1.read();
    TEST_SILENCE({
      TEST_ASSERT_EQUAL(result.isFault(), false);
      TEST_ASSERT_EQUAL(result.isEmpty(), false);
      TEST_ASSERT_EQUAL(result.isValid(), true);
      TEST_ASSERT_WITHIN(result.getReading(), 512, 512);
    });
  }
}
