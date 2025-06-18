#include "inference.h"

//checks if ⊢a and ⊢a->b; if so, then adds ⊢b to knowledge set
int mp(knowledge_set_t *ks, theorem_t *a, theorem_t *b)
{
    theorem_t impl = { .op = IMPLIES, .left = a, .right = b };
    if (contains_theorem(ks, a) && contains_theorem(ks, &impl))
    {
        add_to_knowledge_set(ks, b);
        return 1;
    }
    return 0;
}