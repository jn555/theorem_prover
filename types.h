#ifndef TYPES_H
#define TYPES_H

typedef enum
{
    VARIABLE,
    IMPLIES,
    NEGATION,
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
} theorem_t;

theorem_t* make_neg(theorem_t *child);
theorem_t* make_var(char name);
theorem_t* make_impl(theorem_t *left, theorem_t *right);

#endif