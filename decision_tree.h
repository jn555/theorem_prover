#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "knowledge_set.h"
#include "types.h"
#include "utils.h"

typedef struct decision_node
{
    theorem_t* goal;
    knowledge_set_t* ks;
    struct decision_node* next;
    struct decision_node* prev;
    bool paths [9];
} decision_node_t;

void print_decision_tree(decision_node_t* head);
// void init_decision_tree(decision_node_t* target, theorem_t* goal, knowledge_set_t* ks, decision_node* next, decision_node* prev);

#endif