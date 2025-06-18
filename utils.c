#include <stdio.h>
#include "utils.h"

int compare_trees(theorem_t *a, theorem_t *b)
{
    if (a->op != b->op)
        return 0;
    if (a->op == VARIABLE && b->op == VARIABLE && (a->var_name != b->var_name))
        return 0;
    if (a->op == NEGATION && b->op == NEGATION)
        return compare_trees(a->child, b->child);
    if (a->op == IMPLIES && b->op == IMPLIES)
        return compare_trees(a->left, b->left) && compare_trees(a->right, b->right);

    //if equal
    return 1;
}

void print_theorem(theorem_t *a)
{
    if (a->op == VARIABLE) 
    {
        printf(" %c ", a->var_name);
        return;
    }

    if (a->op == NEGATION)
    {
        printf(" ~");
        print_theorem(a->child);
        return;
    }

    if (a->op == IMPLIES)
    {
        printf("(");
        print_theorem(a->left);
        printf(" -> ");
        print_theorem(a->right);
        printf(")");
        return;
    }
}

