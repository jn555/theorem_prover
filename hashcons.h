#ifndef HASHCONS_H
#define HASHCONS_H

#include <stdint.h>
#include "types.h"

#define TABLE_SIZE 100

extern theorem_t* hashtable[TABLE_SIZE];

//This structure lets me reuse identical subformulas. Whenever I create a formula, it calculates hash value, and if it already exists, it points to the existing one.
//This saved considerable memory and improves efficiency.

void init_hashtable();
static inline uint32_t mix32(uint32_t op, uint32_t target);
uint32_t hash_unary(operator_t op, theorem_t* child);
static inline uint32_t hash_binary(uint32_t op, theorem_t* left, theorem_t* right);
theorem_t* make_var(char name);
theorem_t* make_neg(theorem_t *child);
theorem_t* make_impl(theorem_t* left, theorem_t* right);

#endif