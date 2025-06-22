#include "inference.h"
#include <stdio.h>
#include "axioms.h"

//checks if ⊢a and ⊢a->b; if so, then adds ⊢b to knowledge set
int mp(knowledge_set_t *ks, theorem_t *a, theorem_t *b)
{
    theorem_t* impl = make_impl(a, b);
    printf("Made Impl: ");
    print_theorem(impl);
    printf("\n");
    if (contains_theorem(ks, a) && contains_theorem(ks, impl))
    {
        add_to_knowledge_set(ks, b);
        return 1;
    }
    return 0;
}

int prove(knowledge_set_t *ks, theorem_t *goal)
{
    while (1)
    {
        int old_size = ks->size;
        for (int i = 0; i < old_size; i++)
        {
            theorem_t* temp = ks->data[i];
            for (int j = 0; j < old_size; j++)
            {
                theorem_t* curr = ks->data[j];
                if (curr->op != IMPLIES) continue;
                // printf("Iteration i %d\n", i);
                // printf("Iteration j %d\n", j);
                if (curr->left == temp)
                {
                    theorem_t *new = curr->right;
                    add_to_knowledge_set(ks, new);
                    //add_to_knowledge_set already checks duplicate
                    if (new == goal) return 1;
                }
            }
        }
        if (ks->size == old_size) return 0; //knowledge set not expanded
    }
}

int seek_axiom(knowledge_set_t *ks, theorem_t *goal)
{
    for (int i = 0; i < ax_size; i++)
    {
        //case of the right side of implication axiom matching goal
        //however, for this we need the left side of the axiom satisfied as prereq
        //and modify the variables of the axiom according to this (the left and right side share variables)
        if (axiom_set[i]->op == IMPLIES && compare_trees(axiom_set[i]->right, goal))
        {
            add_to_knowledge_set(ks, generate_modified_axiom(axiom_set[i], goal));
        }
    }
}