/**
 * Paradigm Hyperloop Prototype Node Control Firmware
 *
 * ABOUT:
 * This file defines the controls logic for a single Brake Node.  A Brake Node
 * is special from a generic node in that it is directly connected to all
 * the required sensors and actuators to adequately control the brake Module
 * in the event of an emergency, a crucial element to building a fail-safe
 * vehicle.
 *
 * HARDWARE:
 * This module is designed to control Pod 2 Brakes, with a few changes. The
 * following hardware is expected to be connected to this node:
 *   Actuators
 *    - 1 5/3 Solenoid (2 coils, 3 states - Engage, Closed, Release)
 *   Sensors
 *    - 2 Analog 5V press transducers (Honeywell 150PSI) 1 Tank, 1 Piston
 *    - 2 Analog 5V Sharp Distance Sensors (2-10cm)
 *    - 3 0-5V Cold Comp Thermocouple Amplifiers
 *
 * COMMUNICATIONS:
 * This module assumes an Arduino Compatible Ethernet IC is connected.
 * This module acquires its MAC and IP at compile time.
 *   Defaults:
 *    - MAC_ADDRESS: 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
 *    - IP_ADDRESS: 192, 168, 1, 177
 *   Note:
 *    Static Assignment of MAC and IP address should be investigated, there are
 *    Possibly better alternatives.
 *    - Determine if a MAC can be burned into the NIC once during assembly
 *        -> Then we could avoid compile time and use DHCP.
 *    - Determine if a default MAC is already burned into the NIC
 *    - Determine if we can generate a unique MAC based on the uC SerialUSB #
 *
 * DESIGN:
 * See the infamous KeynoteCAD for design notes on the design of this Prototype
 * controller.
 */

/////////////////////// HEADERS /////////////////////
#include <Arduino.h>
#include <sensor.h>

#include <filters/bias.h>
#include <converters/cubic.h>
#include <detectors/high_low.h>

#ifndef UNIT_TEST  // IMPORTANT LINE!

#define __TEST(cond, a, b) do { \
  if (cond) { \
    SerialUSB.print("[PASS] "); \
    SerialUSB.print(__XSTR__(cond)); \
    SerialUSB.print("\n"); \
  } else { \
    SerialUSB.print("[FAIL] "); \
    SerialUSB.print(__XSTR__(cond)); \
    SerialUSB.print(" actual lhs:"); \
    SerialUSB.print(a); \
    SerialUSB.print(" rhs:"); \
    SerialUSB.print(b); \
    SerialUSB.print("\n"); \
    abort(); \
  } \
} while (0);

#define TEST_ASSERT_EQUAL(a, b) __TEST((a) == (b), a, b)

#define TEST_ASSERT_WITHIN(a, b, e) __TEST(((a) < (b + e) && (a) > (b - e)), a, b)

BiasFilter Fi(0.5);
CubicConverter C(0,0,1,0);
HighLowDetector D(8, 4086);
Sensor S(1, &C, &Fi, &D);

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

void test_high_low_detector() {
  HighLowDetector d1(8, 4086);
  TEST_ASSERT_EQUAL(d1.check(0), SENSOR_FAULT_LOW);
  TEST_ASSERT_EQUAL(d1.check(4095), SENSOR_FAULT_HIGH);
  TEST_ASSERT_EQUAL(d1.check(2000), 0x0);
}


void test_sensor() {
  CubicConverter c3(0.1,0.2,0.3,1.0);
  BiasFilter f4(0.25);
  HighLowDetector d1(8, 4086);

  Sensor s1(10, &c3, &f4, &d1);

  TEST_ASSERT_EQUAL(s1.getChannel(), 10);
  TEST_ASSERT_EQUAL(s1.getValue(), 0.0);

  TEST_ASSERT_EQUAL(s1.isValid(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultLow(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultHigh(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultVariance(), false);
  TEST_ASSERT_EQUAL(s1.hasFaultHW(), false);
  TEST_ASSERT_EQUAL(s1.hasRisen(), false);
}

void test() {
  test_high_low_detector();
  test_bias_filter();
  test_cubic_converter();
  test_sensor();
}

void setup() {
  SerialUSB.begin(115200);

  while (!SerialUSB) {
    ; // Waiting for SerialUSB Connection
  }
  info("SerialUSB Connected!");
  test();
  info("==== ALL TESTS PASSED! ====");
}

void loop() {
  S.addValue(1000);
  info(S.getValue());

  info("Completed Loop");
  // delay
  delay(1000);
}


#else
///////////////////////////////////////////////////////////////////////////////
///                                  TESTS
///////////////////////////////////////////////////////////////////////////////
#endif
