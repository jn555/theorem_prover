#include "knowledge_set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "axioms.h"

void init_knowledge_set(knowledge_set_t *ks)
{
    ks->capacity = 10;
    ks->size = 0;
    //data is pointer to first element of array of pointers
    ks->data = malloc(sizeof(theorem_t *) * ks->capacity);
}

void free_knowledge_set(knowledge_set_t *ks)
{
    free(ks->data);
}

void add_to_knowledge_set(knowledge_set_t *ks, theorem_t *target)
{
    if (contains_theorem(ks, target)) return;
    if (ks->size == ks->capacity)
    {
        ks->capacity *= 2;
        ks->data = realloc(ks->data, sizeof(theorem_t *) * ks->capacity);
    }
    //go to last element of array
    ks->data[ks->size++] = target;
}

void print_knowledge_set(knowledge_set_t *ks)
{
    print_axioms();
    printf("\n");
    if (ks->size == 0) 
    {
        printf(" ⊢ ∅\n");
        return;
    }
    for (size_t i = 0; i < ks->size; ++i)
    {
        printf(" ⊢ ");
        print_theorem(ks->data[i]);
        printf("\n");
    }
}

int contains_theorem(knowledge_set_t *ks, theorem_t* target)
{
    for (size_t i = 0; i < ks->size; ++i)
    {
        if (ks->data[i] == target) return 1;
    }
    //not found
    return 0;
}

knowledge_set_t* clone_knowledge_set (const knowledge_set_t *ks) 
{
  knowledge_set_t *new_ks = malloc(sizeof *new_ks);

  new_ks->capacity = ks->capacity;
  new_ks->size     = ks->size;

  new_ks->data     = malloc(sizeof(theorem_t*) * new_ks->capacity);

  memcpy(new_ks->data, ks->data, sizeof(theorem_t*) * ks->size);

  return new_ks;
}


// Shouldn't be used
// It just copies over the stuff in the axioms struct to the knowledge set (KS)
// (which is not how the axioms should work, as the variables, i.e. a, b, c, could be ANY subformula)
// If you just added them to the KS, then the variables would just be simply variables, not castable to be subformulas
void init_axioms(knowledge_set_t *ks)
{
    for (int i = 0; i < NUM_AXIOMS; i++)
    {
        add_to_knowledge_set(ks, axiom_set[i]);
    }
}