#ifndef AXIOMS_H
#define AXIOMS_H

#include "knowledge_set.h"
#include <stdlib.h>
#include <stdio.h>

#define NUM_AXIOMS 3

extern theorem_t** axiom_set;

void print_axioms();
void init_axiom_set();
//theorem_t* generate_casted_axiom(theorem_t* axiom, theorem_t* source);

#endif