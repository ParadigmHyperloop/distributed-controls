#ifndef PARADIGM_CONFIG_H
#define PARADIGM_CONFIG_H
#include "cdefs.h"
#include "board.h"

/////////////////////// NETWORK /////////////////////

#ifndef MAC_ADDRESS
#define MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#endif

#ifndef IP_ADDRESS
#define IP_ADDRESS 192, 168, 1, 177
#endif

#ifndef LISTEN_PORT
#define LISTEN_PORT 8888 // UDP
#endif

#define MESSAGE_TIMEOUT 1000      // milliseconds
#define PISTON_EMPTY_THRESH 3     // PSI
#define PISTON_AREA 40            // Square Inches
#define VALVE_FLUTTER_MS 100      // milliseconds
#define FORCE_SETPOINT_THRESH 100 // Newtons

#define MAX_SENSORS 16

#define LED(x) digitalWrite(LED_BUILTIN, (x))
/**
 * Number of sensor readings required before a sensor's data is considered
 * valid.
 *
 * Max 15 (0x0F), Min 0 (0x00)
 */
#define SENSOR_COUNT_INIT 0x0F // Require 15 samples before readings are valid

#ifdef DEBUG
#define debug(...) do { \
  Serial.print("[DEBUG] ["); \
  Serial.print(__FUNCTION__); \
  Serial.print(" in "); \
  Serial.print(__FILE__); \
  Serial.print(":"); \
  Serial.print(__LINE__, DEC); \
  Serial.print("] "); \
  Serial.print(__VA_ARGS__); \
  Serial.print("\n"); \
  Serial.flush(); \
} while (0);
#else
#define debug(...)
#endif

// TODO Absract these
#define info(...) do { \
  Serial.print("[INFO] ["); \
  Serial.print(__FUNCTION__); \
  Serial.print(" in "); \
  Serial.print(__FILE__); \
  Serial.print(":"); \
  Serial.print(__LINE__, DEC); \
  Serial.print("] "); \
  Serial.print(__VA_ARGS__); \
  Serial.print("\n"); \
  Serial.flush(); \
} while (0);

#endif
