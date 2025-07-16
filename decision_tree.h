#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "knowledge_set.h"
#include "types.h"
#include "utils.h"

#define NUM_AXIOM_PATHS 7

typedef struct decision_node
{
    theorem_t* goal;
    theorem_t* secondary_goal;
    knowledge_set_t* ks;
    struct decision_node* next;
    struct decision_node* prev;
    bool paths [NUM_AXIOM_PATHS];
} decision_node_t;

void print_decision_tree(decision_node_t* head);
void init_decision_node(decision_node_t* target, 
                        theorem_t* goal,
                        theorem_t* secondary_goal,
                        knowledge_set_t* ks,
                        decision_node_t* next,
                        decision_node_t* prev);
int contains_goal(decision_node_t* target, theorem_t* goal);

#endif