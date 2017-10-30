#ifndef PARADIGM_NODE_H
#define PARADIGM_NODE_H

#include "cdefs.h"
#include "config.h"
#include "sensor.h"
#include "valves.h"

/////////////////////// NODE STATE /////////////////////

#define N_MODES 8

typedef enum node_mode {
  kModeInit = 0,      // Implemented
  kModeStandard = 1,  // Implemented
  kModeReset = 2,     // Implemented
  kModeProgram = 3,   // TODO: Not Implemented
  kModeManual = 4,    // Implemented
  kModeInhibit = 5,   // Implemented
  kModeEmergency = 6, // Implemented
  kModeTest = 7       // TODO: Not Implemented
} node_mode_t;

typedef struct node {
  uint8_t mode;
  sensor_t dist_rear;                 // Rear Pad distance sensor
  sensor_t dist_front;                // Front Pad distance sensor
  sensor_t temp_rear;                 // Rear Pad temp sensor
  sensor_t temp_front;                // Front Pad temp sensor
  sensor_t tank_press;                // Tank Pressure sensor
  sensor_t piston_press;              // Piston Pressure sensor
  sensor_t *sensors[MAX_SENSORS];     // Array of pointers to sensors by channel
  int32_t last_message;               // Timestamp of last message recieved
  int32_t force_setpoint;             // Current Force Setpoint (kilonewtons)
  int32_t ref_press;                  // Atmospheric pressure
  brake_valve_t valve;                // 5/3 Solenoid Valve
  brake_valve_state_t override_state; // Manual Mode desired state
} node_t;

/**
 * Add a sensor to the given node.
 *
 * @param n A pointer to the node to set the value of
 * @param s A pointer to the sensor_t to register with this node.
 *
 * @returns 0 on success, -1 on error (channel conflict)
 */
int node_add_sensor(node_t *n, sensor_t *s);

/**
 * Set the mode of the given node
 *
 * @param n A pointer to the node to update the mode of
 * @param new_mode The new mode to apply to the node
 */
void node_set_mode(node_t *n, node_mode_t new_mode);

/**
 * Validate the given int as a valid node_mode_t.  If the int is a valid
 * representation of a node_mode_t, then assign it to the given out argument
 *
 * @param new_mode An int that represents a node_mode_t (perhaps from a packet)
 * @param out A pointer to a node_mode_t.  If new_mode is valid, this variable
 *            will be populated with a node_mode_t.  Pass NULL if no output is
 *            desired
 *
 * @returns 0 on success, -1 on error.
 */
int node_validate_mode(int new_mode, node_mode_t *out);

#endif
