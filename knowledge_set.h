#ifndef KNOWLEDGE_SET_H
#define KNOWLEDGE_SET_H

#include "hashcons.h"
#include "types.h"
#include "utils.h"

typedef struct {
    theorem_t** data;
    int size;
    int capacity;
} knowledge_set_t;

void init_knowledge_set(knowledge_set_t *ks);
void free_knowledge_set(knowledge_set_t *ks);
void add_to_knowledge_set(knowledge_set_t *ks, theorem_t *target);
void print_knowledge_set(knowledge_set_t *ks);
void init_axioms(knowledge_set_t *ks);

int contains_theorem(knowledge_set_t *ks, theorem_t* target);

#endif