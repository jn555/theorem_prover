#include "types.h"
#include <stdlib.h>

theorem_t* make_neg(theorem_t *child)
{
    theorem_t* neg = malloc(sizeof(theorem_t));
    neg->op = NEGATION;
    neg->child = child;
    return neg;
}

theorem_t* make_var(char name)
{
    theorem_t* var = malloc(sizeof(theorem_t));
    var->op = VARIABLE;
    var->var_name = name;
    return var;
}

theorem_t* make_impl(theorem_t *left, theorem_t *right)
{
    theorem_t* impl = malloc(sizeof(theorem_t));
    impl->op = IMPLIES;
    impl->left = left;
    impl->right = right;
    return impl;
}
