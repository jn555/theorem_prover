#include "queue.h"

void init_queue(queue_t* queue)
{
    queue->size = 0;
    queue->write_ptr = 0;
    queue->read_ptr = 0;
}

int push_queue(queue_t* queue, theorem_t* data)
{
    if (queue->size == QUEUE_SIZE) return 0; //full
    queue->data[queue->write_ptr] = data;
    queue->write_ptr = (queue->write_ptr + 1) % QUEUE_SIZE;
    queue->size++;
    return 1;
}

theorem_t* pop_queue(queue_t* queue)
{
    if (queue->size == 0) return NULL; //empty
    theorem_t* out = queue->data[queue->read_ptr];
    queue->read_ptr = (queue->read_ptr + 1) % QUEUE_SIZE;
    queue->size--;
    return out;
}

void print_queue(queue_t* q) {
    printf("Queue (size=%d):\n", q->size);
    int idx = q->read_ptr;
    for (int cnt = 0; cnt < q->size; ++cnt) {
        print_theorem(q->data[idx]);
        printf("\n");
        idx = (idx + 1) % QUEUE_SIZE;
    }
    printf("––––––––––––––––\n");
}