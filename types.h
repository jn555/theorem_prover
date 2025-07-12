#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

typedef enum
{
    VARIABLE,
    IMPLIES,
    NEGATION,
    GENERIC,
} operator_t;

typedef struct theorem 
{
    operator_t op;

    union {
        struct {
            struct theorem *left, *right;
        };
        struct theorem *child;
        char var_name;
    };
    
    uint32_t hash;
    struct theorem *next;
} theorem_t;

/*
theorem_t* make_neg(theorem_t *child);
theorem_t* make_var(char name);
theorem_t* make_impl(theorem_t *left, theorem_t *right);
*/

#endif