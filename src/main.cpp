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
 *    - Determine if we can generate a unique MAC based on the uC serial #
 *
 * DESIGN:
 * See the infamous KeynoteCAD for design notes on the design of this Prototype
 * controller.
 */

/////////////////////// HEADERS /////////////////////
#include <Arduino.h>
#include "config.h"
#include "network.h"
#include "node.h"

#ifdef ARDUINO_SAMD_ZERO
#include <Adafruit_ASFcore.h>
#include <Adafruit_SleepyDog.h>
#endif

/////////////////////// CONVERTIONS /////////////////////

#define POUND_FORCE_TO_NEWTONS(x) (445 * x) / 100
#define BIAS(old_val, new_val, bias, total)                                    \
  (((((old_val) * (bias) / (total)) +                                          \
     ((new_val) * ((total) - (bias)) / (total)))) /                            \
   (total))

/////////////////////// GLOBAL STATE STORAGE /////////////////////

node_t BRAKE = {0};

/////////////////////// SENSOR HELPERS /////////////////////

int _dist_sensor(sensor_t *s) {
  uint32_t new_val = map(s->raw, 0, 4095, 220, 10); // Bullshit
  s->val = BIAS(s->val, new_val, 0, 255);
  return 0;
}

int _temp_sensor(sensor_t *s) {
  uint32_t new_val = map(s->raw, 0, 4095, 220, 10); // Bullshit
  s->val = BIAS(s->val, new_val, 0, 255);
  return 0;
}

int _press_sensor(sensor_t *s) {
  uint32_t new_val = map(s->raw, 0, 4095, 220, 10); // Bullshit
  s->val = BIAS(s->val, new_val, 0, 255);
  return 0;
}

/////////////////////// CONTROLLER UTILITIES /////////////////////

int32_t calculate_current_force() {
  int32_t relative_100s_psi = BRAKE.piston_press.val - BRAKE.ref_press;

  int32_t force_pounds = (relative_100s_psi * PISTON_AREA) / 100;

  return POUND_FORCE_TO_NEWTONS(force_pounds);
}


/////////////////////// CORE CONTROLLER STEPS /////////////////////
/* Do: Network, Sensors, State, then Outputs */

void handle_network() {
  req_packet_t req = {0};
  memset(&req, 0, sizeof(req)); // TODO: Probably uneeded
  int rc = net_recv_request(&req);

  // is there a packet recieved
  if (rc == 0) {
    return;
  }

  node_set_mode(&BRAKE, (node_mode_t)req.mode);
  if (BRAKE.mode == kModeStandard) {
    BRAKE.force_setpoint = req.arg0;
    BRAKE.ref_press = req.arg1;
  } else if (BRAKE.mode == kModeManual) {
    BRAKE.override_state = (brake_valve_state_t)req.arg0;
  }

  resp_packet_t response;
  memset(&response, 0, sizeof(response)); // TODO: Probably uneeded

  response.mode = BRAKE.mode;
  response.tank_press = BRAKE.tank_press.val;
  response.piston_press = BRAKE.piston_press.val;
  response.dist_front = BRAKE.dist_front.val;
  response.dist_rear = BRAKE.dist_rear.val;
  response.valve_state = BRAKE.valve.state;
  response.temp_front = BRAKE.temp_front.val;
  response.temp_rear = BRAKE.temp_rear.val;
  response.force_setpoint = BRAKE.force_setpoint;

  rc = net_send_response(&response);
  assert(rc == 1);
}

void read_sensors() {
  int i = 0;
  while (BRAKE.sensors[i]) {
    sensor_read(BRAKE.sensors[i]);
    i++;
  }
}

void update_states() {
  if (BRAKE.mode == kModeStandard) {
    if (BRAKE.last_message - millis() > MESSAGE_TIMEOUT) {
      BRAKE.mode = kModeEmergency;
    }
  } else if (BRAKE.mode == kModeReset) {
    if (BRAKE.piston_press.val < PISTON_EMPTY_THRESH) {
      delay(100); // hit watchdog timer
    }
  }
}

static void brake_standard_mode() {
  int32_t current_force = calculate_current_force();
  int32_t desired_force = BRAKE.force_setpoint;
  int32_t err = desired_force - current_force;

  // TODO: Deal with situations when tank pressure is getting low
  // (disallow release until controller sets force_setpoint <= 0)

  // TODO: Deal with overtemp situations?
  // (actually we probably don't want to actively control based on temp)

  // TODO: Deal with position of pad.
  // (Ensure that position front and back indicated pad is extended)
  brake_valve_state_t new_state = BRAKE.valve.state;
  if (abs(err) <= FORCE_SETPOINT_THRESH) {
    // Within Acceptable Range
    new_state = kBrakeValveClosed;
  } else if (err > 0) {
    // Underpressed
    new_state = kBrakeValveReleased;
  } else if (err < 0) {
    // Overpressed
    new_state = kBrakeValveEngaged;
  }

  if (BRAKE.valve.state != new_state) {
    brake_valve_set(&BRAKE.valve, new_state);
  }
}

void output_actuators() {
  switch (BRAKE.mode) {;
    case kModeStandard:
      brake_standard_mode();
      break;
    case kModeManual:
      brake_valve_set(&BRAKE.valve, BRAKE.override_state);
      break;
    case kModeInit:
    case kModeInhibit:
    case kModeReset:
    case kModeTest:
    case kModeProgram:
      // No Actiuations are allowed
      brake_valve_set(&BRAKE.valve, kBrakeValveReleased);
      break;
    case kModeEmergency:
      brake_valve_set(&BRAKE.valve, kBrakeValveEngaged);
      // NOTE: if packet arrives from controller, the emergency is lifted.
      break;
    default:
      abort();
  }
}
void read_serial() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    uint8_t incomingByte = Serial.read();
    debug("Old Mode:");
    debug(BRAKE.mode);
    uint8_t mode = incomingByte - '0';
    node_set_mode(&BRAKE, (node_mode_t)mode);
    debug("New Mode:");
    debug(BRAKE.mode);
  }
}
/////////////////////// SETUP & LOOP /////////////////////

void setup() {
#ifdef ARDUINO_SAMD_ZERO
  // 1 second startup watchdog
  Watchdog.enable(1000);
#endif
  Serial.begin(9600);

  Serial.print("Waiting for Serial Connection\n");
  delay(1000);

  // Initial Mode
  BRAKE.mode = kModeInit;

  info("Configuring Sensors\n");

  // Setup Sensors
  sensor_init(&BRAKE.dist_front, (char *)"dist_front", 0, _dist_sensor);
  sensor_init(&BRAKE.dist_rear, (char *)"dist_rear", 0, _dist_sensor);

  sensor_init(&BRAKE.temp_front, (char *)"temp_front", 0, _temp_sensor);
  sensor_init(&BRAKE.temp_rear, (char *)"temp_rear", 0, _temp_sensor);

  sensor_init(&BRAKE.tank_press, (char *)"tank_press", 0, _press_sensor);
  sensor_init(&BRAKE.piston_press, (char *)"piston_press", 0, _press_sensor);

  // Configure the brake valve
  brake_valve_init(&BRAKE.valve, 1, 2);

  // Set the UDP message timeout
  BRAKE.last_message = millis();
  info("Last Message Set to:");
  info(BRAKE.last_message);

  info("Network Setup");
  net_setup();
  info("Network Setup Done!");
#ifdef ARDUINO_SAMD_ZERO
  // 100ms watchdog.  Auto Reset on hang
  info("Setting WatchDog to 100 -> End info output");
  Watchdog.disable();
  Watchdog.enable(100);
#endif
}

void loop() {
#ifdef ARDUINO_SAMD_ZERO
  Watchdog.reset();
#endif
  debug("--- Starting Network");
  // if there's data available, read a packet
  handle_network();

  debug("--- Read Serial");
  read_serial();

  debug("--- Starting Sensors");
  // Read in and update sensors
  read_sensors();

  debug("--- Starting States");
  // Change controller mode if needed (network timeout, controller reset)
  update_states();

  debug("--- Starting Actuators");
  // Outputs
  output_actuators();

  // delay
  debug("Completed Loop");
  debug(BRAKE.mode);
  debug(BRAKE.valve.state);
  delay(10);
}
