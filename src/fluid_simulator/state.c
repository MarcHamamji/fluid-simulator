#include "state.h"

void state_free(State *state) {
    free(state->particles);
}

