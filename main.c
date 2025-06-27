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
    
    //this init axioms function I have commented below should not be used. I
    // It just copies over the stuff in the axioms struct to the knowledge set (KS)
    // (which is not how the axioms should work, as the variables, i.e. a, b, c, could be ANY subformula)
    // If you just added them to the KS, then the variables would just be simply variables, not castable to be subformulas
    
    //init_axioms(ks);

    printf("______________________________________________________________________________________________________________\n");
    print_knowledge_set(ks);

    theorem_t* a = make_var('a');
    theorem_t* b = make_var('b');
    theorem_t* c = make_var('c');
    
    theorem_t* a_impl_a = make_impl(a, a);
    // theorem_t* a_impl_b = make_impl(a, b);
    // theorem_t* b_impl_c = make_impl(b, c);
    // theorem_t* a_impl_c = make_impl(a, c);
    // add_to_knowledge_set(ks, a_impl_b);
    // add_to_knowledge_set(ks, b_impl_c);
    // add_to_knowledge_set(ks, a_impl_b);
    
    printf("Result of proof: %d", prove_backwards(ks, a_impl_a));
    print_knowledge_set(ks);
    free_knowledge_set(ks);
}