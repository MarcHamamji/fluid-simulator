#include "particle_linked_list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParticleLinkedList particle_linked_list_new() {
  return (ParticleLinkedList){
      .head = NULL,
  };
}

void particle_linked_list_add(ParticleLinkedList *list, Particle *particle) {

  ParticleLinkedListNode *node = malloc(sizeof(ParticleLinkedListNode));
  node->particle = particle;
  node->next = list->head;

  list->head = node;
}

static void particle_linked_list_node_free(ParticleLinkedListNode *node) {
  if (node == NULL)
    return;

  particle_linked_list_node_free(node->next);
  free(node);
}

void particle_linked_list_clear(ParticleLinkedList *list) {
  particle_linked_list_node_free(list->head);
  list->head = NULL;
}

void particle_linked_list_free(ParticleLinkedList *list) {
  particle_linked_list_node_free(list->head);
}
