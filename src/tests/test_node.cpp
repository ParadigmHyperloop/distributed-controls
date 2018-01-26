#include "tests.h"

void test_node() {
  Node n("TestNode", RUN);

  TEST_ASSERT_EQUAL(n.getName().compareTo("TestNode"), 0);
  TEST_ASSERT_NOTEQUAL(n.getName().compareTo("TestNode2"), 0);

  n.setName("TestNode2");
  TEST_ASSERT_NOTEQUAL(n.getName().compareTo("TestNode"), 0);
  TEST_ASSERT_EQUAL(n.getName().compareTo("TestNode2"), 0);

  CubicConverter c1(0,0,1,5.5);
  BiasFilter f1(0.25);
  HighLowDetector d1(8, 4086);
  InternalSensorDriver sd1(1);

  CubicConverter c2(0,0,1,5.5);
  BiasFilter f2(0.25);
  HighLowDetector d2(8, 4086);
  InternalSensorDriver sd2(2);

  Sensor s1("s1", &sd1, &c1, &f1, &d1);
  Sensor s2("s2", &sd2, &c2, &f2, &d2);

  TEST_ASSERT_EQUAL((long)n.getSensorByName("s1"), (long)NULL);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s2"), (long)NULL);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s3"), (long)NULL);

  TEST_ASSERT_EQUAL(n.registerSensor(&s1), true);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s1"), (long)&s1);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s2"), (long)NULL);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s3"), (long)NULL);

  TEST_ASSERT_EQUAL(n.registerSensor(&s2), true);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s1"), (long)&s1);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s2"), (long)&s2);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s3"), (long)NULL);

  TEST_ASSERT_EQUAL(n.unregisterSensor(&s1), true);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s1"), (long)NULL);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s2"), (long)&s2);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s3"), (long)NULL);

  TEST_ASSERT_EQUAL(n.unregisterSensor(&s2), true);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s1"), (long)NULL);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s2"), (long)NULL);
  TEST_ASSERT_EQUAL((long)n.getSensorByName("s3"), (long)NULL);
}
