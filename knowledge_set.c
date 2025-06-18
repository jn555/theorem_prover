#include "knowledge_set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void init_knowledge_set(knowledge_set_t *ks)
{
    ks->capacity = 10;
    ks->size = 0;
    //data is pointer to first element of array of pointers
    ks->data = malloc(sizeof(theorem_t *) * ks->capacity);
}

void free_knowledge_set(knowledge_set_t *ks)
{
    for (size_t i = 0; i < ks->size; ++i)
    {
        free(ks->data[i]);
    }
    free(ks->data);
}

void add_to_knowledge_set(knowledge_set_t *ks, theorem_t *target)
{
    if (ks->size == ks->capacity)
    {
        ks->capacity *= 2;
        ks->data = realloc(ks->data, sizeof(theorem_t) * ks->capacity);
    }
    //go to last element of array
    ks->data[ks->size++] = target;
}

void print_knowledge_set(knowledge_set_t *ks)
{
    for (size_t i = 0; i < ks->size; ++i)
    {
        printf("⊢ ");
        print_theorem(ks->data[i]);
        printf("\n");
    }
}

int contains_theorem(knowledge_set_t *ks, theorem_t* target)
{
    for (size_t i = 0; i < ks->size; ++i)
    {
        //printf("checking for: ");
        //print_theorem(ks->data[i]);
        //printf("\n");
        if (compare_trees(ks->data[i], target))
        {
            return 1;
        }
    }
    //not found
    return 0;
}

void init_axioms(knowledge_set_t *ks)
{
    //axiom 1
    theorem_t* temp_a = make_var('a');
    theorem_t* temp_b = make_var('b');
    theorem_t* temp_impl = make_impl(temp_b, temp_a);
    theorem_t* ax_1= make_impl(temp_a, temp_impl);



    add_to_knowledge_set(ks, temp_a);
    add_to_knowledge_set(ks, ax_1);
    //add_to_knowledge_set(ks, temp_a);
}