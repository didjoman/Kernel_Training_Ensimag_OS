#ifndef PROCESS_PRIORITY_QUEUE_H
#define PROCESS_PRIORITY_QUEUE_H

#include "uptime.h"
#include "process.h"
#include "stdbool.h"

struct proc_priority_queue{
        struct Process* head;
	struct Process* tail;
};

void push_p(struct proc_priority_queue* queue, struct Process* p);
struct Process* pop_p(struct proc_priority_queue* queue, time_t get_up);
bool empty_p(struct proc_priority_queue* queue);


#endif

