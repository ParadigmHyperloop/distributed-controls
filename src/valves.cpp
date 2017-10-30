#include "valves.h"

int brake_valve_set(brake_valve_t *b, brake_valve_state new_state) {
  if (brake_is_locked(b)) {
    debug("Valve Locked");
    return -1;
  }

  debug("Setting Valve:");
  debug(new_state);

  int engage_val = (new_state == kBrakeValveEngaged ? HIGH : LOW);
  digitalWrite(b->engage_pin, engage_val);

  int release_val = (new_state == kBrakeValveReleased ? HIGH : LOW);
  digitalWrite(b->release_pin, release_val);

  b->last_update = millis();
  b->state = new_state;
  return 0;
}

bool brake_is_locked(brake_valve_t *b) {
  return (millis() - b->last_update < VALVE_FLUTTER_MS);
}

void brake_valve_init(brake_valve_t *b, int engage_pin, int release_pin) {
  b->engage_pin = engage_pin;
  b->release_pin = release_pin;
  b->last_update = 0;
  brake_valve_set(b, kBrakeValveClosed);
}
