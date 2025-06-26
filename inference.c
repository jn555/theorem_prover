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
        case VARIABLE: {
            //make it never return null?
            //return find_mapping(m, axiom->var_name);
            theorem_t *sub = find_mapping(m, axiom->var_name);
            return sub ? sub : axiom;   // ← never return NULL
        }
        case IMPLIES:
            return make_impl(generate_modified_axiom(m, axiom->left), generate_modified_axiom(m, axiom->right));
        case NEGATION:
            return make_neg(generate_modified_axiom(m, axiom->child));
    }
    return axiom;
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

//iterate through each ks element a, if ⊢a and ⊢a -> b, add the b to the knowledge set
int mp(knowledge_set_t *ks, theorem_t *goal)
{
    theorem_t* outer;
    theorem_t* inner;
    for (int i = 0; i < ks->size; i++)
    {
        // ⊢a
        outer = ks->data[i];
        for (int j = 0; j < ks->size; j++)
        {
            inner = ks->data[j];
            //if there is an ⊢a->b
            if (inner->op == IMPLIES && inner->left == outer)
            {
                add_to_knowledge_set(ks, inner->right);
                if (inner->right == goal) return 1;
            }
        }
    }
    return 0;
}


//the main prove function im working on, it infinite loops though.
//the point is to see if i can form the goal (i.e. the formula im trying to prove) from the RHS of an axiom (as all axioms are implications)
//If the LHS of the axiom (with the variables set as the appropriate subformulas to match the goal) is in the knowledge set, then u've proven ur goal.
// If not, then set the LHS of the axiom to the goal (you're trying to derive that now).
// As that will give you a string of implications which you can use MP for to get your goal.

//However, it currently infinite loops in adding axioms. This is due to a core issue with logic
int prove_backwards(knowledge_set_t *ks, theorem_t* final_goal)
{
    queue_t* goals = malloc(sizeof(queue_t));
    init_queue(goals);

    knowledge_set_t *seen_goals = malloc(sizeof(knowledge_set_t));
    init_knowledge_set(seen_goals);
 
    subst_map_t subst_map;

    push_queue(goals, final_goal);

    while (goals->size > 0)
    {
        if (mp(ks, final_goal)) return 1;
        theorem_t* goal = pop_queue(goals);
        if (contains_theorem(seen_goals, goal))
            continue;
        add_to_knowledge_set(seen_goals, goal);
        for (int i = 0; i < ax_size; i++)
        {
            subst_map_init(&subst_map);
            if (fit_onto_axiom(&subst_map, axiom_set[i]->right, goal))
            {
                theorem_t* new_theorem = generate_modified_axiom(&subst_map, axiom_set[i]);
                add_to_knowledge_set(ks, new_theorem);
                if (mp(ks, goal)) return 1;
                //now we just need the LHS of the axiom so that we can get the goal
                // push_queue(goals, new_theorem->left);
                if (!contains_theorem(seen_goals, new_theorem->left))
                {
                    add_to_knowledge_set(seen_goals, new_theorem->left);
                    push_queue(goals, new_theorem->left);
                }
            }
            print_queue(goals);
        }
    }
    return 0;
}
