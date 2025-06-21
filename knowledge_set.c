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
        if (ks->data[i] == target) return 1;
    }
    //not found
    return 0;
}

void init_axioms(knowledge_set_t *ks)
{
    theorem_t* temp_a = make_var('a');
    theorem_t* temp_b = make_var('b');
    theorem_t* temp_c = make_var('c');

    //Axiom 1
    //a → (b → a)
    theorem_t* temp_impl = make_impl(temp_b, temp_a);
    theorem_t* ax_1= make_impl(temp_a, temp_impl);

    //Axiom 2
    //(A → (B → C)) → ((A → B) → (A → C))
    theorem_t* temp_impl_1 = make_impl(temp_b, temp_c);
    //b -> c
    theorem_t* temp_impl_2 = make_impl(temp_a, temp_impl_1);
    //a -> (b -> c)
    theorem_t* temp_impl_3 = make_impl(temp_a, temp_b);
    //a -> b
    theorem_t* temp_impl_4 = make_impl(temp_a, temp_c);
    //a -> c
    theorem_t* temp_impl_5 = make_impl(temp_impl_3, temp_impl_4);
    //(a -> b) -> (a -> c)
    theorem_t* ax_2 = make_impl(temp_impl_2, temp_impl_5);

    //Axiom 3
    theorem_t* temp_a_neg = make_neg(temp_a);
    theorem_t* temp_b_neg = make_neg(temp_b);
    theorem_t* temp_impl_6 = make_impl(temp_b_neg, temp_a_neg);
    theorem_t* ax_3 = make_impl(temp_impl_6, temp_impl_3);

    add_to_knowledge_set(ks, ax_1);
    add_to_knowledge_set(ks, ax_2);
    add_to_knowledge_set(ks, ax_3);
}