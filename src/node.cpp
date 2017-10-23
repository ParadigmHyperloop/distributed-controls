#include "node.h"

void add_sensor(node_t *n, sensor_t *s) {
  assert(n);
  assert(s);
  assert(n->sensors[s->channel] == NULL);
  n->sensors[s->channel] = s;
}

void node_set_mode(node_t * n, node_mode_t new_mode) {
  if (new_mode >= 0 && new_mode < N_MODES) {
    n->mode = new_mode;
  }
}
