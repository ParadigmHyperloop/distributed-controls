#ifndef PARADIGM_SENSOR_H
#define PARADIGM_SENSOR_H

#include "cdefs.h"
#include "config.h"

#define SENSOR_FAULT_LOW 0x1
#define SENSOR_FAULT_HIGH 0x2
#define SENSOR_FAULT_VARIANCE 0x4
#define SENSOR_FAULT_HW 0x8

#define SENSOR_SET_HIGH(s) APPLY_MASK(s->stat, SENSOR_FAULT_HIGH)
#define SENSOR_CLEAR_HIGH(s) REMOVE_MASK(s->stat, SENSOR_FAULT_HIGH)

#define SENSOR_SET_LOW(s) APPLY_MASK(s->stat, SENSOR_FAULT_LOW)
#define SENSOR_CLEAR_LOW(s) REMOVE_MASK(s->stat, SENSOR_FAULT_LOW)

#define SENSOR_SET_VAR(s) APPLY_MASK(s->stat, SENSOR_FAULT_VARIANCE)
#define SENSOR_CLEAR_VAR(s) REMOVE_MASK(s->stat, SENSOR_FAULT_VARIANCE)

#define SENSOR_SET_HW(s) APPLY_MASK(s->stat, SENSOR_FAULT_HW)
#define SENSOR_CLEAR_HW(s) REMOVE_MASK(s->stat, SENSOR_FAULT_HW)

#define SENSOR_COUNT_RESET(s) (s->stat | SENSOR_COUNT_INIT)
#define SENSOR_COUNT_DEC(s) (s->stat & 0x0F) && (s->stat--);
#define SENSOR_COUNT(s) (s->stat & 0x0F)
/////////////////////// SENSOR ABSTRACTIONS /////////////////////

struct sensor;
typedef int (*processor)(struct sensor *sensor);

/**
 * A bitmask that defines the state of the sensor.
 * This is essentail for ensuring the validity of data
 *
 *  - The 4 LSB form a 4 bit count-down counter. sensor_init() sets this
 *    counter to 15 (0xF) and the counter is decremented on each sensor_read().
 *    The counter stops when it gets to 0 and thus, after the first 16 reads,
 *    the 4 MSBs will be 0.
 *  - The 4 MSB form a 4 bit mask of fault bits. When the bit is set (0x1), the
 *    fault is present and registered, when the bit is 0, the fault is not
 *    present.  In a proper functioning sensor, these 4 fault bits are 0.
 *    The bits are defined as follows in MSB -> LSB order
 *     - SENSOR_FAULT_HW: A hardware fault has been detected with the sensor.
 *       e.x. I2C Sensor stopped acknoledging.
 *     - SENSOR_FAULT_VARIANCE: The sensor has been varying unacceptably. This
 *       is the most generic fault and is really up to the processing function
 *       to determine what varaiance is acceptable and what is unacceptable
 *       (should the processing function even decide to compute variance)
 *     - SENSOR_FAULT_LOW: The sensor is reading out of range, specifically it
 *       is reading values that are too low. LOW is defined as the computed
 *       real-world sensor reading (psi, cm, etc) and not elecrical readings
 *       (0.01 volts, 5 raw adc). Note this is because some sensors are inverse
 *       and therefore it is useless to a user to say the ADC is reading low
 *       when that could actually mean that the HP pressure transducer went
 *       out of range with a high computed reading
 *     - SENSOR_FAULT_HIGH: The sensor is reading out of range, specifically it
 *       is reading values that are too high. For example: the HP Pressure
 *       transudcer is reading 2500 PSI but it is only rated for 2250. This is
 *       either an overpressure event or the sensor is shorted to 5V or Ground.
 *
 * HELPERS:
 * helper macros for the 4 fault bits exist to set and unset the fault bits.
 *
 *  sensor_t *s = ...;
 *  // Sensor is out of range
 *  SENSOR_SET_LOW(s);
 *  // Sensor is back in range for an acceptable time (you can reset the
 *  // countdown again if desired)
 *  SENSOR_CLEAR_LOW(s);
 *
 */
typedef uint8_t sensor_stat_t;

typedef struct sensor {
  char name[16];
  uint8_t channel;
  int32_t raw;
  int32_t val;
  sensor_stat_t stat;
  processor process;
} sensor_t;

/**
 * Initialize the sensor struct with a name, channel number, and processing
 * function.
 *
 * @param s A pointer to the uninitialized sensor struct
 * @param name A null-terminated string (15 char max) with a human readable
               name for debugging and logging. This string is copied.
 * @param channel The ADC channel this sensor lives on (ex. AIN3)
 * @param processor A function that processes the raw ADC value into a usable
               value
 */
void sensor_init(sensor_t *s, char *name, uint8_t channel, processor p);

/**
 * Read in a new value for the sensor and update the sensor
 *
 * @param s A pointer to the sensor to read and update
 *
 * @return 0 on success, -1 on failure
 */
int sensor_read(sensor_t *s);

/**
 * Determines if the sensor readings are valid or not
 *  - Checks whether the number of samples is signifigant
 *  - Checks the fault bits on the sensor
 *
 * @param s A pointer to the sensor to read and update
 *
 * @return 0 if the sensor is OK, otherwise an error code
 */
inline int sensor_valid(sensor_t *s) { return s->stat; }

/**
 * Gets the current reading of the sensor
 *
 * @param s A pointer to the sensor to read and update
 *
 * @return the value of the sensor currently
 */
int32_t sensor_get(sensor_t *s);

#endif
