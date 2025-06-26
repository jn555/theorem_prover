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

//ignore this prove() function, it was forward proving attempt, which is not goal directed, so its pretty much just random search until we get goal, and lotta times wont reach goal (not efficient)
//and ALSO if it matches the left hand side of an axiom, you could derive the RHS of the axiom with MP, just make sure the variables match, as the LHS may not have all of the vars within it (more may be present in RHS, so mapping might not work)
int prove(knowledge_set_t *ks, theorem_t *goal)
{
    while (1)
    {
        //Stage 1: iterate through each ks element a, if ⊢a and ⊢a -> b, add the b to the knowledge set
        int old_size = ks->size;
        for (int i = 0; i < old_size; i++)
        {
            theorem_t* temp = ks->data[i];
            for (int j = 0; j < old_size; j++)
            {
                theorem_t* curr = ks->data[j];
                if (curr->op == IMPLIES && curr->left == temp)
                {
                    add_to_knowledge_set(ks, curr->right);
                    //add_to_knowledge_set already checks duplicate
                    if (curr->right == goal) return 1;
                }
            }
        }

        //Stage 2
        subst_map_t subst_map;
        for (int i = 0; i < old_size; i++)
        {
            theorem_t* temp = ks->data[i];
            if (temp->op != IMPLIES) continue;
            for (int j = 0; j < ax_size; j++)
            {
                //Stage 2: for each theorem in KS (if it's an implication) and the left side of it matches an axiom, cast the axiom and insert it, which lets us derive the RHS
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
int prove_backwards(knowledge_set_t *ks, queue_t* goals, theorem_t* final_goal)
{
    subst_map_t subst_map;

    push_queue(goals, final_goal);

    while (goals->size > 0)
    {
       theorem_t* goal = pop_queue(goals);
        if (mp(ks, goal)) return 1;
        print_knowledge_set(ks);
        for (int i = 0; i < ax_size; i++)
        {
            subst_map_init(&subst_map);
            if (fit_onto_axiom(&subst_map, axiom_set[i]->right, goal))
            {
                theorem_t* new_theorem = generate_modified_axiom(&subst_map, axiom_set[i]);
                add_to_knowledge_set(ks, new_theorem);
                //now we just need the LHS of the axiom so that we can get the goal
                if (!contains_theorem(ks, new_theorem->left)) push_queue(goals, new_theorem->left);
            }
        }
    }
    return 0;
}