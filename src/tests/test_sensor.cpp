#include "tests.h"

void test_sensor() {
  CubicConverter c2(0,0,1,5.5);
  BiasFilter f4(0.25);
  HighLowDetector d1(8, 4086);
  InternalSensorDriver sd1(1);

  Sensor s1("test_sensor(s1)", &sd1, &c2, &f4, &d1);

  TEST_ASSERT_EQUAL(s1.getValue(), 0.0);

  TEST_ASSERT_EQUAL(s1.isValid(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultLow(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultHigh(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultVariance(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultHW(), false);
  TEST_ASSERT_EQUAL(s1.hasRisen(), false);
  s1.addValue(0);
  TEST_ASSERT_EQUAL(s1.hasFaultLow(), true);
  s1.addValue(4095);
  TEST_ASSERT_EQUAL(s1.hasFaultHigh(), true);
  TEST_ASSERT_EQUAL(s1.hasFaultLow(), false);

  // Ensure that the sensor does not rise when fed HIGH faults
  for (int i = 0; i < 2 * f4.getRiseTime(0.99); i++) {
    s1.addValue(4095);
  }

  TEST_ASSERT_EQUAL(s1.hasRisen(), false);
  TEST_ASSERT_EQUAL(s1.isValid(), false);

  // Feed good data
  for (int i = 0; i < 2 * f4.getRiseTime(0.99); i++) {
    s1.addValue(2000);
  }

  // Ensure that the value rises and becomes valid
  TEST_ASSERT_EQUAL(s1.hasRisen(), true);
  TEST_ASSERT_EQUAL(s1.isValid(), true);
  // Ensure that the value approaches the correct value
  TEST_ASSERT_WITHIN(s1.getValue(), 2005.5, 1.0);

  // Feed a single fault sample
  // TODO: The Fault detector should be more robust
  // TODO: require more than single fault to trip detector
  s1.addValue(4095);

  // Ensure that the fault is detected
  TEST_ASSERT_EQUAL(s1.hasRisen(), false);
  TEST_ASSERT_EQUAL(s1.isValid(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultHigh(), true);

  TEST_ASSERT_EQUAL(s1.getName().compareTo("test_sensor(s1)"), 0);

  TEST_ASSERT_NOTEQUAL(s1.getName().compareTo("not_a_test_sensor"), 0);
  TEST_ASSERT_EQUAL(s1.getName(), "test_sensor(s1)");

  s1.setName("test_sensorB");
  TEST_ASSERT_EQUAL(s1.getName(), "test_sensorB");
}
