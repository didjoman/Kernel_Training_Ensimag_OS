#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#include "process.h"
#include "stdbool.h"

struct proc_queue{
        struct Process* head;
        struct Process* tail;
};

void push(struct proc_queue* queue, struct Process* p);
struct Process* pop(struct proc_queue* queue);
bool empty(struct proc_queue* queue);


#endif

