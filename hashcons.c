#include "hashcons.h"
#include <stdlib.h>

theorem_t* hashtable[TABLE_SIZE];

void init_hashtable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
}

static inline uint32_t mix32(uint32_t op, uint32_t target)
{
    //0x9e3779b9 = golden ratio
    op ^= target + 0x9e3779b9 + (op << 6) + (op >> 2);
    return op;
}

uint32_t hash_unary(operator_t op, theorem_t* child)
{
    return mix32((uint32_t)op, child->hash);
}

static inline uint32_t hash_binary(uint32_t op, theorem_t* left, theorem_t* right)
{
    uint32_t h = op;
    h = mix32(h, left->hash);
    h = mix32(h, right->hash);
    return h;
}

theorem_t* make_var(char name)
{
    uint32_t h = mix32(VARIABLE, (uint32_t)(unsigned char) name);
    uint32_t index = h % TABLE_SIZE;
    for (theorem_t *curr = hashtable[index]; curr != NULL; curr = curr->next)
    {
        if (curr->op == VARIABLE && curr->var_name == name)
            return curr;  // already exists
    }

    theorem_t* new_theorem = malloc(sizeof(theorem_t));
    new_theorem->op = VARIABLE;
    new_theorem->var_name = name;
    new_theorem->hash = h;

    //chain previous after it
    new_theorem->next = hashtable[index];
    hashtable[index] = new_theorem;

    return new_theorem;
}

theorem_t* make_neg(theorem_t *child)
{
    uint32_t h = hash_unary(NEGATION, child);
    uint32_t index = h % TABLE_SIZE;
    // Lookup phase
    for (theorem_t *curr = hashtable[index]; curr != NULL; curr = curr->next)
    {
        if (curr->op == NEGATION && curr->child == child)
            return curr;  // already exists
    }

    theorem_t* new_theorem = malloc(sizeof(theorem_t));
    new_theorem->op = NEGATION;
    new_theorem->child = child;
    new_theorem->hash = h;

    //chain previous after it
    new_theorem->next = hashtable[index];
    hashtable[index] = new_theorem;

    return new_theorem;
}

theorem_t* make_impl(theorem_t* left, theorem_t* right)
{
    uint32_t h = hash_binary(IMPLIES, left, right);
    uint32_t index = h % TABLE_SIZE;
    for (theorem_t *curr = hashtable[index]; curr != NULL; curr = curr->next)
    {
        if (curr->op == IMPLIES && curr->left == left && curr->right == right)
            return curr;  // already exists
    }

    theorem_t* new_theorem = malloc(sizeof(theorem_t));
    new_theorem->op = IMPLIES;
    new_theorem->left = left;
    new_theorem->right = right;
    new_theorem->hash = h;

    //chain previous after it
    new_theorem->next = hashtable[index];
    hashtable[index] = new_theorem;

    return new_theorem;
}




