
#include "sensor.h"

void sensor_init(sensor_t *s, char *name, uint8_t channel, processor p) {
  int i = 0;
  while (name[i] != '\0' && i < 15) {
    s->name[i] = name[i];
    i++;
  }
  s->name[i] = '\0';
  s->channel = channel;
  s->raw = 0;
  debug(channel, DEC);
  SENSOR_COUNT_RESET(s);

  s->process = p;
  s->process(s);
  debug("sensor init done!");
}

void sensor_destroy(sensor_t *s) {
  // Nothing
  // be sure to remove from node_t *sensors[];
}

int sensor_read(sensor_t *s) {
  s->raw = analogRead(s->channel);
  s->process(s);

  // The high 4 bits of the stat count the number of samples so far
  SENSOR_COUNT_DEC(s);
  return 0;
}
