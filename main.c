#include <stdio.h>
#include <stdlib.h>

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

int main()
{
    theorem_t *impl = malloc(sizeof(theorem_t));
    theorem_t *impl_1 = malloc(sizeof(theorem_t));
    theorem_t *basic_p = malloc(sizeof(theorem_t));
    theorem_t *basic_q = malloc(sizeof(theorem_t));

    impl->op = IMPLIES;
    impl->left = basic_p;
    impl->right = basic_q;

    impl_1->op = IMPLIES;
    impl_1->left = impl;
    impl_1->right = basic_q;


    basic_p->op = VARIABLE;
    basic_q->op = VARIABLE;

    basic_p->var_name = 'p';
    basic_q->var_name = 'q';

    print_theorem(impl_1);

    free(basic_p);
    free(basic_q);
    free(impl);
    free(impl_1);
}