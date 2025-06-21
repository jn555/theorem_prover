#include "inference.h"

//checks if ⊢a and ⊢a->b; if so, then adds ⊢b to knowledge set
int mp(knowledge_set_t *ks, theorem_t *a, theorem_t *b)
{
    theorem_t* impl = make_impl(a, b);
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
            for (int j = 0; j < old_size; j++)
            {
                // printf("The theorem is for i [%d]: \n", i);
                // printf("\t");
                // print_theorem(ks->data[i]);
                // printf("\nThe theorem for j [%d]\t", j);
                // print_theorem(ks->data[j]);
                // printf("\n");
                //exit early if did MP and the second arg is the goal, that means it found goal
                if (mp(ks, ks->data[i], ks->data[j]) && ks->data[j] == goal) return 1;
            }
        }
        if (contains_theorem(ks, goal)) return 1;
        if (ks->size == old_size)  // no progress
            return 0;
    }
}