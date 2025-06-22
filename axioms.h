#ifndef AXIOMS_H
#define AXIOMS_H

#include "knowledge_set.h"
#include <stdlib.h>
#include <stdio.h>

#define INIT_AXIOM_SIZE 3

extern theorem_t** axiom_set;
extern int ax_size;

void print_axioms();
void init_axiom_set();
void init_axioms_1();
void add_to_axioms(theorem_t* target);
theorem_t* generate_modified_axiom(theorem_t* axiom, theorem_t* goal);

#endif