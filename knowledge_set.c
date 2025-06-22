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

void init_axioms(knowledge_set_t *ks)
{
    for (int i = 0; i < ax_size; i++)
    {
        add_to_knowledge_set(ks, axiom_set[i]);
    }
}