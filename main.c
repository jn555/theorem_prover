#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

#include "knowledge_set.h"
#include "inference.h"
#include "axioms.h"

int main()
{
    init_hashtable();
    knowledge_set_t *ks = malloc(sizeof(knowledge_set_t));
    init_knowledge_set(ks);
    print_knowledge_set(ks);
    init_axioms_1();
    //init_axioms(ks);

    printf("______________________________________________________________________________________________________________\n");
    print_knowledge_set(ks);

    theorem_t* temp_a = make_var('a');
    theorem_t* temp_b = make_var('b');

    theorem_t* temp_impl_a_b_pre = make_impl (temp_a, temp_b);

    theorem_t* temp_impl_a_b = make_impl (temp_impl_a_b_pre, temp_b);

    theorem_t* temp_impl_b_a = make_impl (temp_b, temp_impl_a_b);
    
    //a -> (b -> a)
    theorem_t* temp_impl_1 = make_impl (temp_impl_a_b, temp_impl_b_a);

    // (a -> (b -> a)) -> (b -> a)
    theorem_t* temp_impl_2 = make_impl (temp_impl_1, temp_impl_a_b);

    //a -> b
    //(a -> b) -> b
    //add_to_knowledge_set(ks, temp_a);
    //add_to_knowledge_set(ks, temp_impl);
    //add_to_knowledge_set(ks, temp_impl);
    //add_to_knowledge_set(ks, temp_impl_4);
    add_to_knowledge_set(ks, temp_impl_2);
    //add_to_knowledge_set(ks, temp_impl_1);

    // print_knowledge_set(ks);
    printf("Result of Prove: %d", prove(ks, temp_impl_a_b));
    //printf("Result of Prove 2: %d", prove(ks, temp_impl_2));
    //printf("Result of Prove: %d", prove(ks, temp_a));
    //printf("MP Result: %d \n", mp(ks, temp_a, temp_impl_1));
    printf("______________________________________________________________________________________________________________\n");
    print_knowledge_set(ks);

    printf("______________________________________________________________________________________________________________\n");


    // printf("The expression is: \t");
    // print_theorem(temp_impl_4); 
    // printf("The axiom it: \t");
    // print_theorem(axiom_set[0]); 
    // printf("\n");
    // subst_map_t subst_map;
    // subst_map_init(&subst_map);
    // print_subst_map(&subst_map);
    // printf("The result of fitting onto axiom is: %d\n", fit_onto_axiom(&subst_map, axiom_set[0], temp_impl_4));
    // print_subst_map(&subst_map);
    // //add here
    // print_theorem(generate_modified_axiom(&subst_map, axiom_set[0]));
    
    free_knowledge_set(ks);
}