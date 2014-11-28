#include "process_queue.h"


/**
 * Add Process in tail
 * @param queue is the queue we want to add the process to.
 * @param p is a pointer to the process to insert on the queue 
 */
void push(struct proc_queue* queue, struct Process* p)
{
	// If the queue is empty, the new proc will be the first and last one
	if(empty(queue)){
		queue->head = p;
		queue->tail = p;
	} else {
		// Insert the process in tail.
		queue->tail->next = p;
		queue->tail = queue->tail->next;
	}
	// We make sure that the process has no next proc, since it is the last.
	p->next = NULL;
}

/**
 * Pop the first element of the queue
 * @param queue is the queue we want to add the process to.
 */
struct Process* pop(struct proc_queue* queue)
{
	struct Process* tmp = queue->head;
	// Update the new head of the list;
	if(!empty(queue))
		queue->head = queue->head->next;

	// The process is removed from the list, so it has next proc any more
	if(tmp != NULL)
		tmp->next = NULL;

	return tmp;
}


/**
 * @param queue is the queue we want to add the process to.
 * @return true if the queue is empty, false else.
 */
bool empty(struct proc_queue* queue)
{
	return queue->head == NULL;
}
