#pragma once

#include "../utils/vector.h"
#include "particle_linked_list.h"

typedef struct SpacePartitioningGrid {
  ParticleLinkedList **grid;

  Vector window_size;

  Vector grid_size;
  int grid_length;
  Vector cell_size;

} SpacePartitioningGrid;

SpacePartitioningGrid space_partitioning_grid_new(Vector cell_size,
                                                  Vector window_size);

void space_partitioning_grid_add_particle(SpacePartitioningGrid *grid,
                                          Particle *particle);

float space_partitioning_grid_accumulate_over_neighbors(
    SpacePartitioningGrid *grid, Vector *source, State* state,
    float (*callback)(Vector *source, Particle *neighbor, State *state));

void space_partitioning_grid_clear_cells(SpacePartitioningGrid *grid);

void space_partitioning_grid_free(SpacePartitioningGrid *grid);
