#include "decision_tree.h"


// void init_decision_tree(decision_node_t* target, theorem_t* goal, knowledge_set_t* ks, struct decision_node* next, struct decision_node* prev)
// {
//     target->goal = goal;
//     target->ks = ks;
//     target->next = next;
//     target->prev = prev;
// }


void init_decision_node(decision_node_t* target, 
                        theorem_t* goal,
                        knowledge_set_t* ks,
                        decision_node_t* next,
                        decision_node_t* prev)
{
    target->goal = goal;
    target->ks   = ks;
    target->next = next;
    target->prev = prev;
    // clear all branch flags
    for (int i = 0; i < NUM_AXIOM_PATHS; ++i)
        target->paths[i] = false;
}

void print_decision_tree(decision_node_t* head)
{
    printf("printing decision tree:\n");
    decision_node_t* curr = head;
    while (curr != NULL)
    {
        print_theorem(curr->goal);
        curr = curr->prev;
        printf("\n");
    }
    printf("\n");
}