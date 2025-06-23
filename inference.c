#include "inference.h"
#include <stdio.h>
#include "axioms.h"


void subst_map_init(subst_map_t *m) 
{
  m->size = 0;
}

//find the associated theorem for a char
theorem_t* find_mapping(subst_map_t *m, char target)
{
    for (int i = 0; i < m->size; i++)
    {
        if (m->key[i] == target)
        {
            return m->value[i];
        }
    }
    return NULL;
}

//1 if bound succesfully, 0 if not
int add_mapping(subst_map_t *m, char key, theorem_t* theorem)
{
    theorem_t* check = find_mapping(m, key);
    if (check == theorem)
        return 1;
    else if (check != NULL)
        return 0;
    
    if (m->size == MAX_AXIOM_VARS) return 0;
    m->key[m->size] = key;
    m->value[m->size] = theorem;
    m->size++;
    return 1;
}

int fit_onto_axiom(subst_map_t* mapping, theorem_t* axiom, theorem_t* target)
{
    if (axiom->op != target->op) return 0;
    switch (axiom->op)
    {
        case VARIABLE:
            return add_mapping(mapping, axiom->var_name, target);
        case NEGATION:
            return fit_onto_axiom(mapping, axiom->child, target->child);
        case IMPLIES:
            return fit_onto_axiom(mapping, axiom->left, target->left) && fit_onto_axiom(mapping, axiom->right, target->right);
        default:
            return 0;
    }
}

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
                //seek_axiom(ks, temp, goal);
            }
        }
        if (ks->size == old_size) return 0; //knowledge set not expanded
    }
}

/*
int seek_axiom_direct(knowledge_set_t *ks, theorem_t *source, theorem_t *goal)
{
    for (int i = 0; i < ax_size; i++)
    {
        //case of the right side of implication axiom matching goal
        //however, for this we need the left side of the axiom satisfied as prereq
        //and modify the variables of the axiom according to this (the left and right side share variables)

        //THERE IS AN ISSUE THOUGH, its not structure matching, the variables have to match relative to the formulas (names dont matter, but relative)
        //i.e. a-> (b->a) axiom matches structurally with a-> (b->c) but NOT the same, even with arbitrary var axiom, as the diff variable names signify distinct subformulas within it
        if (axiom_set[i]->op == IMPLIES && compare_trees(axiom_set[i]->left, source) && compare_trees(axiom_set[i]->right, goal))
        {
            add_to_knowledge_set(ks, make_impl(source, goal));
            add_to_knowledge_set(ks, goal);
            return 1;
        }
    }
    return 0;
}

int seek_axiom_indirect(knowledge_set_t *ks, theorem_t *source)
{
    for (int i = 0; i < ax_size; i++)
    {
        if (axiom_set[i]->op == IMPLIES && compare_trees(axiom_set[i]->left, source))
        {
            add_to_knowledge_set(ks, generate_casted_axiom(axiom_set[i], source));
        }
    }
}

*/