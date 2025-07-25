#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "utils.h"

#define QUEUE_SIZE 100

typedef struct
{
    void* data[QUEUE_SIZE];
    int size;
    int read_ptr;
    int write_ptr;
} queue_t;


void init_queue(queue_t* queue);
int push_queue(queue_t* queue, theorem_t* data);
theorem_t* pop_queue(queue_t* queue);
void print_queue(queue_t* queue);

#endif