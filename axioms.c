#include "axioms.h"

static int ax_cap;
theorem_t** axiom_set = NULL;
int ax_size = 0;

void init_axiom_set()
{
    axiom_set = malloc(sizeof(theorem_t *) * NUM_AXIOMS);
    ax_size = 0;


    theorem_t* temp_a = make_var('a');
    theorem_t* temp_b = make_var('b');
    theorem_t* temp_c = make_var('c');

    //Axiom 1
    //a → (b → a)
    theorem_t* temp_impl = make_impl(temp_b, temp_a);
    theorem_t* ax_1= make_impl(temp_a, temp_impl);

    //Axiom 2
    //(A → (B → C)) → ((A → B) → (A → C))
    theorem_t* temp_impl_1 = make_impl(temp_b, temp_c);
    //b -> c
    theorem_t* temp_impl_2 = make_impl(temp_a, temp_impl_1);
    //a -> (b -> c)
    theorem_t* temp_impl_3 = make_impl(temp_a, temp_b);
    //a -> b
    theorem_t* temp_impl_4 = make_impl(temp_a, temp_c);
    //a -> c
    theorem_t* temp_impl_5 = make_impl(temp_impl_3, temp_impl_4);
    //(a -> b) -> (a -> c)
    theorem_t* ax_2 = make_impl(temp_impl_2, temp_impl_5);

    //Axiom 3
    theorem_t* temp_a_neg = make_neg(temp_a);
    theorem_t* temp_b_neg = make_neg(temp_b);
    theorem_t* temp_impl_6 = make_impl(temp_b_neg, temp_a_neg);
    theorem_t* ax_3 = make_impl(temp_impl_6, temp_impl_3);

    axiom_set[0] = ax_1;
    axiom_set[1] = ax_2;
    axiom_set[2] = ax_3;
}

void print_axioms()
{
    printf("{");
    for (int i = 0; i < NUM_AXIOMS; i++)
    {
        print_theorem(axiom_set[i]);
        printf(" ,");
    }
    printf("}");
}

//make it so that axioms have a diff equality operator, so that it actually adds to the knowledge set the correct variable axiom


/*
// so basically the LHS of source matches the LHS structure of axiom, so 
theorem_t* generate_casted_axiom(theorem_t* axiom, theorem_t* source)
{


}
*/
