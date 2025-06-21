#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

#include "knowledge_set.h"
#include "inference.h"

int main()
{
    init_hashtable();
    knowledge_set_t *ks = malloc(sizeof(knowledge_set_t));
    init_knowledge_set(ks);
    print_knowledge_set(ks);
    init_axioms(ks);
    
    print_knowledge_set(ks);

    theorem_t* temp_a = make_var('a');
    theorem_t* temp_b = make_var('b');
    theorem_t* temp_impl = make_impl (temp_a, temp_b);
    theorem_t* temp_impl_1 = make_impl (temp_b, temp_a);

    theorem_t* goal = make_impl (temp_a, temp_b);

    //a -> b
    //(a -> b) -> b
    add_to_knowledge_set(ks, temp_a);
    add_to_knowledge_set(ks, temp_impl);
    //add_to_knowledge_set(ks, temp_impl_1);

    print_knowledge_set(ks);
    printf("Result of Prove: %d", prove(ks, temp_impl_1));
    //printf("MP Result: %d \n", mp(ks, temp_a, temp_impl_1));
    printf("\n");
    print_knowledge_set(ks);


    //free_knowledge_set(ks);
}