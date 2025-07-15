#ifndef INFERENCE_H
#define INFERENCE_H

#define MAX_AXIOM_VARS  3   // enough for a,b,c

#include "knowledge_set.h"
#include "types.h"
#include "queue.h"
#include "decision_tree.h"

//with each iteration step, reset this. it's for adding axioms
typedef struct {
    char        key[MAX_AXIOM_VARS];
    theorem_t*  value[MAX_AXIOM_VARS];
    int         size;
} subst_map_t;

int mp(knowledge_set_t *ks, theorem_t *goal);
int prove(knowledge_set_t *ks, theorem_t *goal);
int prove_backwards(knowledge_set_t *ks, theorem_t* final_goal);
int prove_with_tree(decision_node_t* curr, theorem_t* final_goal);

void subst_map_init(subst_map_t *m);
//void subst_map_reset(subst_map_t *m);
theorem_t* find_mapping(subst_map_t *m, char target);
int add_mapping(subst_map_t *m, char key, theorem_t* theorem);
int fit_onto_axiom(subst_map_t* mapping, theorem_t* axiom, theorem_t* target);
void print_subst_map(subst_map_t *m);
theorem_t* generate_modified_axiom(subst_map_t *m, theorem_t* axiom);

#endif