#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

#include "knowledge_set.h"
#include "inference.h"

int main()
{
    knowledge_set_t *ks = malloc(sizeof(knowledge_set_t));
    init_knowledge_set(ks);
    print_knowledge_set(ks);
    init_axioms(ks);
    
    print_knowledge_set(ks);

    theorem_t* temp_a = make_var('a');
    theorem_t* temp_b = make_var('b');
    theorem_t* temp_impl = make_impl (temp_b, temp_a);
    //a -> b
    //(a -> b) -> b


    printf("MP Result: %d \n", mp(ks, temp_a, temp_impl));
    printf("\n");

    print_knowledge_set(ks);

    free_knowledge_set(ks);
}