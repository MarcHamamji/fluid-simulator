#include "space_partitioning_grid.h"
#include "particle_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

SpacePartitioningGrid space_partitioning_grid_new(Vector cell_size,
                                                  Vector window_size) {

  Vector grid_size_not_ceiled = vector_divide(&window_size, &cell_size);
  Vector grid_size = vector_ceil(&grid_size_not_ceiled);
  int grid_length = grid_size.x * grid_size.y;

  SpacePartitioningGrid grid = (SpacePartitioningGrid){
      .window_size = window_size,
      .cell_size = cell_size,
      .grid_size = grid_size,
      .grid_length = grid_length,
      .grid = calloc(grid_length, sizeof(ParticleLinkedList)),
  };

  for (int i = 0; i < grid_length; i++) {
    grid.grid[i] = malloc(sizeof(ParticleLinkedList));
    *grid.grid[i] = particle_linked_list_new();
  }

  return grid;
}

static Vector
space_partitioning_grid_get_cell_coordinates(SpacePartitioningGrid *grid,
                                             Vector *position) {

  Vector cell_coordinates_not_floored =
      vector_divide(position, &grid->cell_size);

  Vector cell_coordinates = vector_floor(&cell_coordinates_not_floored);

  return cell_coordinates;
}

static int space_partitioning_grid_get_index(SpacePartitioningGrid *grid,
                                             Vector cell_coordinates) {
  int index = cell_coordinates.y * grid->grid_size.x + cell_coordinates.x;
  return index;
}

void space_partitioning_grid_add_particle(SpacePartitioningGrid *grid,
                                          Particle *particle) {
  Vector cell_coordinates =
      space_partitioning_grid_get_cell_coordinates(grid, &particle->position);

  int index = space_partitioning_grid_get_index(grid, cell_coordinates);

  particle_linked_list_add(grid->grid[index], particle);
}

float space_partitioning_grid_accumulate_over_neighbors(
    SpacePartitioningGrid *grid, Vector *source, State* state,
    float (*callback)(Vector *source, Particle *neighbor, State *state)) {

  Vector cell_coordinates =
      space_partitioning_grid_get_cell_coordinates(grid, source);

  float accumulator = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      Vector offset = vector_new(i, j);
      Vector neighbor_cell_coordinate = vector_add(&cell_coordinates, &offset);

      if (!vector_is_in_rect(&neighbor_cell_coordinate, &grid->grid_size))
        continue;

      int index =
          space_partitioning_grid_get_index(grid, neighbor_cell_coordinate);

      ParticleLinkedList *particle_linked_list = grid->grid[index];
      ParticleLinkedListNode *current = particle_linked_list->head;

      while (current != NULL) {
        accumulator += callback(source, current->particle, state);
        current = current->next;
      }
    }
  }

  return accumulator;
}

void space_partitioning_grid_clear_cells(SpacePartitioningGrid *grid) {
  for (int i = 0; i < grid->grid_length; i++) {
    particle_linked_list_clear(grid->grid[i]);
  }
}

void space_partitioning_grid_free(SpacePartitioningGrid *grid) {
  for (int i = 0; i < grid->grid_length; i++) {
    particle_linked_list_free(grid->grid[i]);
  }
  free(grid->grid);
}
