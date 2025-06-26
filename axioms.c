#include "axioms.h"

static int ax_cap;
theorem_t** axiom_set = NULL;
int ax_size = 0;

void init_axiom_set()
{
    axiom_set = malloc(sizeof(theorem_t *) * INIT_AXIOM_SIZE);
    ax_cap = INIT_AXIOM_SIZE;
    ax_size = 0;
}

void print_axioms()
{
    printf("{");
    for (int i = 0; i < ax_size; i++)
    {
        print_theorem(axiom_set[i]);
        printf(" ,");
    }
    printf("}");
}

//make it so that axioms have a diff equality operator, so that it actually adds to the knowledge set the correct variable axiom

void add_to_axioms(theorem_t* target)
{
    if (ax_size == ax_cap)
    {
        ax_cap *= 2;
        axiom_set = realloc(axiom_set, (sizeof(theorem_t *) * ax_cap));  
    }
    axiom_set[ax_size] = target;
    ax_size++;
}

/*
// so basically the LHS of source matches the LHS structure of axiom, so 
theorem_t* generate_casted_axiom(theorem_t* axiom, theorem_t* source)
{


}
*/

void init_axioms_1()
{
    init_axiom_set();
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

    add_to_axioms(ax_1);
    add_to_axioms(ax_2);
    add_to_axioms(ax_3);
}