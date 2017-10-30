#ifndef PARADIGM_VALVES_H
#define PARADIGM_VALVES_H

#include "cdefs.h"
#include "config.h"

/////////////////////// VALVE ABSTRACTIONS /////////////////////

/* Generic 2-way valve state enumeration */
typedef enum valve_state {
  kValveFault = 0,   // Valve has faulted, unknown state
  kValveOpen = 1,    // Valve is open
  kValveClosed = 2,  // Valve is closed
  kValveOpening = 3, // Valve is opening, not confirmed opened
  kValveClosing = 4  // Valve is closing, not confirmed closed
} valve_state_t;

/* Abstraction of a standard 2/2, 3/2, or other 2-way solenoid */
typedef struct valve {
  valve_state_t state;
  int pin;
  uint32_t last_update;
} valve_t;

/* A Brake valve is a standard 5/3 solenoid */
typedef enum brake_valve_state {
  kBrakeValveFault = 0,     // Valve has faulted, unknown state
  kBrakeValveEngaged = 1,   // Valve is engaging air to brake
  kBrakeValveClosed = 2,    // Valve is closed
  kBrakeValveReleased = 3,  // Valve is releasing air from brake
  kBrakeValveEngaging = 4,  // Valve is traveling to engaged state
  kBrakeValveClosing = 5,   // Valve is traveling to closed state
  kBrakeValveReleasing = 6, // Valve is traveling to released state
} brake_valve_state_t;

/* Abstraction of a 5/3 solenoid valve */
typedef struct brake_valve {
  brake_valve_state_t state; // The current state of the valve
  int engage_pin;            // GPIO pin for the release coil
  int release_pin;           // GPIO pin for the release coil
  uint32_t last_update; // Timestamp of last time the brake state was changed
} brake_valve_t;

/**
 * Set the state of the given valve to the state provided
 *
 * @return -1 on failure (valve locked out), 0 on success
 */
int brake_valve_set(brake_valve_t *b, brake_valve_state new_state);

/**
 * Determine if the valve is locked out because it was recently updated
 */
bool brake_is_locked(brake_valve_t *b);

/**
 * Initialize the brake valve
 *
 * @param b Pointer to the brake_valve_t to initialize
 * @param engage_pin The pin number of the engage solenoid coil
 * @param release_pin The pin number of the release solenoid coil
 */
void brake_valve_init(brake_valve_t *b, int engage_pin, int release_pin);

#endif
