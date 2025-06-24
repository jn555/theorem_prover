#include "inference.h"
#include <stdio.h>
#include "axioms.h"


/*
void subst_map_reset(subst_map_t *m) 
{
  for (int i = 0; i < m->size; ++i)
  {
    m->key[i] = 0;
    m->value[i] = NULL;
  }
  m->size = 0;
}
*/

void subst_map_init(subst_map_t *m) 
{
  m->size = 0;
}

void print_subst_map(subst_map_t *m) 
{
  for (int i = 0; i < m->size; i++)
  {
    printf("%d\t%c\t%p", i, m->key[i], m->value[i]);
    print_theorem(m->value[i]);
    printf("\n");
  }
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

theorem_t* generate_modified_axiom(subst_map_t *m, theorem_t* axiom)
{
    switch (axiom->op)
    {
        case VARIABLE:
            return find_mapping(m, axiom->var_name);
        case IMPLIES:
            return make_impl(generate_modified_axiom(m, axiom->left), generate_modified_axiom(m, axiom->right));
        case NEGATION:
            return make_neg(generate_modified_axiom(m, axiom->child));
    }
}

//given some input axiom, see if it can be used to generate the target formula, and thus be added to the knowledge set
int fit_onto_axiom(subst_map_t* mapping, theorem_t* axiom, theorem_t* target)
{
    if (axiom->op != target->op && axiom->op != VARIABLE) return 0;
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

//BASICALLY, if its an implication and the left side of it matches an axiom, cast the axiom and insert it, so we can get the RHS
//and ALSO if it matches the left hand side of an axiom, you could derive the RHS of the axiom with MP, just make sure the variables match, as the LHS may not have all of the vars within it (more may be present in RHS, so mapping might not work)
//iterate through each ks element a, if ⊢a and ⊢a -> b, add the b to the knowledge set
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
        //adding axioms
        subst_map_t subst_map;
        subst_map_init(&subst_map);
        for (int i = 0; i < old_size; i++)
        {
            theorem_t* temp = ks->data[i];
            if (temp->op != IMPLIES) continue;
            for (int j = 0; j < ax_size; j++)
            {
                subst_map_init(&subst_map);
                if (fit_onto_axiom(&subst_map, axiom_set[j], temp->left))
                {
                    add_to_knowledge_set(ks, generate_modified_axiom(&subst_map, axiom_set[j]));
                    add_to_knowledge_set(ks, temp->right);
                    if (temp->right == goal) return 1;
                }
            }
        }
        if (ks->size == old_size) return 0; //knowledge set not expanded
    }
}

