#include "inference.h"
#include <stdio.h>
#include "axioms.h"
#include<unistd.h>

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
    if (check == theorem) //already mapped to this
        return 1;
    else if (check != NULL) //conflict
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
            theorem_t *sub = find_mapping(m, axiom->var_name); //here sub might have nothing, there might be no mappign for that var
            return sub ? sub : make_generic();   // ← never return NULL
        }
        case IMPLIES:
            return make_impl(generate_modified_axiom(m, axiom->left), generate_modified_axiom(m, axiom->right));
        case NEGATION:
            return make_neg(generate_modified_axiom(m, axiom->child));
        default: //the axioms dont have generics
            return NULL;
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
/*
int prove_backwards(knowledge_set_t *ks, theorem_t* final_goal)
{
    queue_t* goals = malloc(sizeof(queue_t));
    init_queue(goals);

    knowledge_set_t *seen_goals = malloc(sizeof(knowledge_set_t));
    init_knowledge_set(seen_goals);
 
    subst_map_t subst_map;

    push_queue(goals, final_goal);
    print_queue(goals);

    int RIGHT_ITERS = 1;
    int increment_right_iters = 0;
    theorem_t* sub_axiom;

    while (goals->size > 0)
    {
        if (mp(ks, final_goal)) return 1;
        theorem_t* goal = pop_queue(goals);

        add_to_knowledge_set(seen_goals, goal);
        for (int i = 0; i < NUM_AXIOMS; i++)
        {
            subst_map_init(&subst_map);
            sub_axiom = axiom_set[i];
            for (int j = 0; j < RIGHT_ITERS; j++)
            {
                if (sub_axiom->op == IMPLIES)
                    sub_axiom = sub_axiom->right;
            }
            printf("Number of iters: %d, Axiom we are comparing for: \t", RIGHT_ITERS);
            print_theorem(sub_axiom);
            printf("\n");
            if (fit_onto_axiom(&subst_map, sub_axiom, goal))
            {
                printf("axiom %d\n", i);
                theorem_t* new_theorem = generate_modified_axiom(&subst_map, sub_axiom); //issue if not variables bound
                add_to_knowledge_set(ks, new_theorem);
                if (mp(ks, goal)) return 1;
                //now we just need the LHS of the axiom so that we can get the goal
                // push_queue(goals, new_theorem->left);
                for (int j = 0; j < RIGHT_ITERS; j++)
                {
                    if (!contains_theorem(seen_goals, new_theorem->left))
                    {
                        printf("Adding to seen goals: ");
                        print_theorem(new_theorem->left);
                        printf("\n");
                        add_to_knowledge_set(seen_goals, new_theorem->left);
                        push_queue(goals, new_theorem->left);
                        increment_right_iters = 1;
                    }
                    new_theorem = new_theorem->right;
                }
            }
            print_queue(goals);
        }
        if (increment_right_iters == 0)
            RIGHT_ITERS++;
        else
            increment_right_iters = 0;
    }
    return 0;
}

*/

//what if for different branches it needs the same goal, which gets combined to final one, but you need those rpevious ones too?

//eliminate cycles by checking the path's KS
int prove_with_tree(decision_node_t* curr, theorem_t* final_goal)
{
    sleep(1);
    if (!curr) return 0;
    if(curr->goal != NULL) print_decision_tree(curr);

    //if (contains_theorem(seen_goals, curr->goal)) return prove_with_tree(curr->prev, seen_goals);

    if (mp(curr->ks, curr->goal)) return 1;

    subst_map_t subst_map;

    //decision path group 1: if you can directly form the goal with an axiom
    for (int i = 0; i < NUM_AXIOMS; i++) //0 -> 2
    {
        if (curr->paths[i]) continue;
        curr->paths[i] = true;

        subst_map_init(&subst_map);
        //if not already explored, and you can form the goal with the axiom directly
        if (fit_onto_axiom(&subst_map, axiom_set[i], curr->goal))
        {
            printf("\nWithin First\n");
            //if axiom fits onto it, then take that as decision edge
            theorem_t* new_theorem = generate_modified_axiom(&subst_map, axiom_set[i]); //issue if not variables bound
            if (contains_theorem(curr->ks, new_theorem)) continue;
            knowledge_set_t* new_ks = clone_knowledge_set(curr->ks);
            add_to_knowledge_set(new_ks, new_theorem);
            decision_node_t* next = malloc(sizeof(decision_node_t));

            curr->next = next;

            init_decision_node(next, curr->secondary_goal, NULL, new_ks, NULL, curr);

    

            // next->prev = curr;
            // next->next = NULL;
            // next->ks = new_ks;
            // next->goal = NULL; //already reached goal
            //print_decision_tree(curr);
            // if (prove_with_tree(next, seen_goals))     // only return if child finds a proof
            //if (mp(next->ks, final_goal)) return 1;
            if (mp(next->ks, final_goal)) return 1;
            else if (next->goal != NULL) return prove_with_tree(next, final_goal);
            else return 0; 
            //return 1; //may be wrong, i wanna do MP
        }
    }

    printf("Decision path 1 failed\n");

    //decision path 2: adding axiom whos RHS matches of goal
    for (int i = 0; i < NUM_AXIOMS; i++)
    {
        if (curr->paths[i+3]) continue; //3->5
        curr->paths[i+3] = true;

        subst_map_init(&subst_map);
        if (fit_onto_axiom(&subst_map, axiom_set[i]->right, curr->goal))
        {
            theorem_t* new_theorem = generate_modified_axiom(&subst_map, axiom_set[i]);
            if (contains_goal(curr, new_theorem->left)) continue; //prevent cycles in decision tree
            if (contains_theorem(curr->ks, new_theorem)) continue; //prevent duplication in KS
            //if axiom fits onto it, then take that as decision edge
            knowledge_set_t* new_ks = clone_knowledge_set(curr->ks);
            add_to_knowledge_set(new_ks, new_theorem);

            decision_node_t* next = malloc(sizeof(decision_node_t));
            curr->next = next;
            init_decision_node(next, new_theorem->left, curr->secondary_goal, new_ks, NULL, curr);

            return prove_with_tree(next, final_goal);
        }
    }

    printf("Decision path 2 failed\n");

    //decision path 3: adding axiom whos RHS of RHS matches of goal (just ax 2)
    if (!curr->paths[6])
    {
        curr->paths[6] = true;
        subst_map_init(&subst_map);
        if (fit_onto_axiom(&subst_map, axiom_set[1]->right->right, curr->goal))
        {
            //if axiom fits onto it, then take that as decision edge
            theorem_t* new_theorem = generate_modified_axiom(&subst_map, axiom_set[1]); //issue if not variables bound
            if (!contains_goal(curr, new_theorem->left)) //prevent cycles in decision tree
            {
                knowledge_set_t* new_ks = clone_knowledge_set(curr->ks);
                add_to_knowledge_set(new_ks, new_theorem);

                decision_node_t* next_1 = malloc(sizeof(decision_node_t));
                //decision_node_t* next_2 = malloc(sizeof(decision_node_t));

                curr->next = next_1;

                init_decision_node(next_1, new_theorem->left, new_theorem->right->left, new_ks, NULL, curr);

                //init_decision_node(next_2, new_theorem->right->left, new_ks, NULL, curr);
 
                /*
                next_next->goal = new_theorem->right->left;
                next_next->prev = next;
                next_next->ks = new_ks;
                next_next->next = NULL;
                */
                return prove_with_tree(next_1, final_goal);
            }
        }
    }

    printf("Decision path 3 failed\n");
    
    //if cant do any of above, then backtrack
    if (curr->prev == NULL)
        return 0;
    

    return prove_with_tree(curr->prev, final_goal);
    //gotta somehow keep track of which decision we took and take other
    //return 0;
}


