#pragma once

#include "particle.h"

typedef struct ParticleLinkedListNode {
  Particle *particle;
  struct ParticleLinkedListNode *next;
} ParticleLinkedListNode;

typedef struct ParticleLinkedList {
  ParticleLinkedListNode *head;
} ParticleLinkedList;

ParticleLinkedList particle_linked_list_new();

void particle_linked_list_add(ParticleLinkedList *list, Particle *particle);

void particle_linked_list_clear(ParticleLinkedList *list);

void particle_linked_list_free(ParticleLinkedList *list);
