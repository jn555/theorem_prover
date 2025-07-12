#include <stdio.h>
#include "utils.h"

int compare_trees(theorem_t *a, theorem_t *b)
{
    if (a->op != b->op)
        return 0;
    if (a->op == NEGATION && b->op == NEGATION)
    {
        if (a->child != NULL && b->child != NULL)
        {
            return compare_trees(a->child, b->child);
        }
        else
        {
            return 0;
        }
    }
    if (a->op == IMPLIES && b->op == IMPLIES)
    {
        if (a->left != NULL && b->right != NULL)
        {
            return compare_trees(a->left, b->left) && compare_trees(a->right, b->right);
        }
        else
        {
            return 0;
        }
    }

    //if equal
    return 1;
}

void print_theorem(theorem_t *a)
{
    if (a->op == GENERIC) 
    {
        printf(" □ ");
        return;
    }

    if (a->op == VARIABLE) 
    {
        printf(" %c ", a->var_name);
        return;
    }

    if (a->op == NEGATION)
    {
        printf(" ⌝");
        print_theorem(a->child);
        return;
    }

    if (a->op == IMPLIES)
    {
        printf("(");
        print_theorem(a->left);
        printf(" → ");
        print_theorem(a->right);
        printf(")");
        return;
    }
}

