#include "state.h"
#include <stdlib.h>

void state_free(State *state) {
  for (int i = 0; i < state->particles_number; i++) {
    free(state->particles[i]);
  }
  free(state->particles);
  space_partitioning_grid_free(&state->space_partitioning_grid);
}
