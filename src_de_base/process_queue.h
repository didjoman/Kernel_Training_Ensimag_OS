#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#include "process.h"
#include "stdbool.h"

struct proc_queue{
        struct process_t* head;
        struct process_t* tail;
};

void push(struct proc_queue* queue, struct process_t* p);
struct process_t* pop(struct proc_queue* queue);
bool empty(struct proc_queue* queue);


#endif

