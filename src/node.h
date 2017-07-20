#ifndef PARADIGM_NODE_H
#define PARADIGM_NODE_H

#include "cdefs.h"
#include "config.h"
#include "sensor.h"
#include "valves.h"


/////////////////////// NODE STATE /////////////////////
typedef enum node_mode {
  kModeInit = 0, // Implemented
  kModeStandard = 1, // Implemented
  kModeReset = 2, // Implemented
  kModeProgram = 3, // TODO: Not Implemented
  kModeManual = 4, // Implemented
  kModeInhibit = 5, // Implemented
  kModeEmergency = 6, // Implemented
  kModeTest = 7 // TODO: Not Implemented
} node_mode_t;

typedef struct node {
  uint8_t mode;
  sensor_t dist_rear; // Rear Pad distance sensor
  sensor_t dist_front; // Front Pad distance sensor
  sensor_t temp_rear; // Rear Pad temp sensor
  sensor_t temp_front; // Front Pad temp sensor
  sensor_t tank_press; // Tank Pressure sensor
  sensor_t piston_press; // Piston Pressure sensor
  sensor_t *sensors[MAX_SENSORS]; // Array of pointers to sensors in channel order
  int32_t last_message; // Timestamp of last message recieved
  int32_t force_setpoint; // Current Force Setpoint (kilonewtons)
  int32_t ref_press;  // Atmospheric pressure
  brake_valve_t valve; // 5/3 Solenoid Valve
  brake_valve_state_t override_state; // Manual Mode desired state
} node_t;

void add_sensor(node_t *n, sensor_t *s);
#endif
