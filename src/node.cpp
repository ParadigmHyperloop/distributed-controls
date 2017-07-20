#include "node.h"

void add_sensor(node_t *n, sensor_t *s) {
  assert(n);
  assert(s);
  assert(n->sensors[s->channel] == NULL);
  n->sensors[s->channel] = s;
}
