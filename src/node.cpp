#include "node.h"

int node_add_sensor(node_t *n, sensor_t *s) {
  if (n && s) {
    if (n->sensors[s->channel] == NULL) {
      n->sensors[s->channel] = s;
      return 0;
    } else {
      error("Attempted to add sensor to node but channel was occupied");
    }

  }
  return -1;
}

void node_set_mode(node_t *n, node_mode_t new_mode) { n->mode = new_mode; }

int node_validate_mode(int new_mode, node_mode_t *out) {
  if (new_mode >= 0 && new_mode < N_MODES) {
    if (out) {
      *out = (node_mode_t)new_mode;
    }
    return 0;
  }
  return -1;
}
