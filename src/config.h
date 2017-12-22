#ifndef PARADIGM_CONFIG_H
#define PARADIGM_CONFIG_H
#include "board.h"
#include "cdefs.h"

/////////////////////// NETWORK /////////////////////

#ifndef MAC_ADDRESS
#define MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#endif

#ifndef IP_ADDRESS
#define IP_ADDRESS 192, 168, 0, 90
#endif

#ifndef LISTEN_PORT
#define LISTEN_PORT 8888 // UDP
#endif

/* If a mesage is not recieved within this amount of time, emergency state */
#define MESSAGE_TIMEOUT 1000 // milliseconds
/* The differential pressure at which the piston is considered empty */
#define PISTON_EMPTY_THRESH 3 // PSI
/* The total surface area of the pistons */
#define PISTON_AREA 40 // Square Inches
/* The amount of time it takes for the solenoid to fully actuate */
#define VALVE_FLUTTER_MS 100 // milliseconds
/* The acceptable error for the force setpoint */
#define FORCE_SETPOINT_THRESH 100 // Newtons
/* The primary watchdog timer, if the loop takes longer than this, reset */
#define WATCHDOG_TIMEOUT 100 // milliseconds
/* The watchdog for the setup function */
#define STARTUP_WATCHDOG 4000 // milliseconds
/* The SerialUSB Baud Rate. Note this strongly affects the watchdog timers */
#define SerialUSB_BAUD 9600 // Baud


/* The delay to allow a SerialUSB client to connect for debug logging */
#define BOOT_DELAY 1000 // milliseconds

#if STARTUP_WATCHDOG < BOOT_DELAY + BOOT_DELAY + 1000
#error "STARTUP_WATCHDOG should be increased or BOOT_DELAY decreased"
#endif

#define MAX_SENSORS 16

#define LED(x) digitalWrite(LED_BUILTIN, (x))
/**
 * Number of sensor readings required before a sensor's data is considered
 * valid.
 *
 * Max 15 (0x0F), Min 0 (0x00)
 */
#define SENSOR_COUNT_INIT 0x0F // Require 15 samples before readings are valid

#define output(type, ...)                                                      \
  do {                                                                         \
    SerialUSB.print("[");                                                         \
    SerialUSB.print(millis());                                                    \
    SerialUSB.print("] ");                                                        \
    SerialUSB.print("[");                                                         \
    SerialUSB.print((type));                                                      \
    SerialUSB.print("] [");                                                       \
    SerialUSB.print(__FUNCTION__);                                                \
    SerialUSB.print(" in ");                                                      \
    SerialUSB.print(__FILE__);                                                    \
    SerialUSB.print(":");                                                         \
    SerialUSB.print(__LINE__, DEC);                                               \
    SerialUSB.print("] ");                                                        \
    SerialUSB.print(__VA_ARGS__);                                                 \
    SerialUSB.print("\n");                                                        \
    SerialUSB.flush();                                                            \
  } while (0);

#ifdef DEBUG
#define debug(...) output("DEBUG", __VA_ARGS__)
#else
#define debug(...)
#endif

#define info(...) output("INFO", __VA_ARGS__)
#define warn(...) output("WARNI", __VA_ARGS__)
#define error(...) output("ERROR", __VA_ARGS__)
#define fault(...)                                                             \
  do {                                                                         \
    output("FAULT", __VA_ARGS__);                                              \
    abort();                                                                   \
  } while (0);

#endif
