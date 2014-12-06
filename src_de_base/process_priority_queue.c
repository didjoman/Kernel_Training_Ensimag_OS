#include "process_priority_queue.h"

/**
 * Add Process at the right place in the queue ( i.e. after the last process 
 * finishing earlier)
 * @param queue is the queue of process we want to add the process into.
 * @param p is a pointer to the process we want to insert in the queue. 
 */
void push_p(struct proc_priority_queue* queue, struct process_t* p)
{

	// Look for the right place to insert the new process
	// Depending on it's get up time.
	struct process_t* last = NULL;
	struct process_t* curr = queue->head;
	while(curr && (curr->get_up < p->get_up)){
		last = curr;
		curr = curr -> next;
	}

	// Insert in head of the queue
	if(last == NULL){
		// we insert in head and link the other elemts
		struct process_t* tmp = curr;
		queue->head = p;
		queue->head->next = tmp;	
	} else {	
		// Insert the process between 2 others, or in tail
		// (process inserted after the last process finishing earlier)
		last->next = p;
		p->next = curr;
	}

	// If the is inserted in end, we need to update the tail of the queue
	if(!curr)
		queue->tail = p;

}

/**
 * Pop the first element of the queue for wich the get-up-time is 
 * inferior or equal to the get-up-time passed in parameter.
 * Or NULL if there is none.
 * @param queue is the queue of process.
 * @param get_up is the max admissible get_up time for the process to pop.
 */
struct process_t* pop_p(struct proc_priority_queue* queue, time_t get_up)
{
	struct process_t* tmp = queue->head;

	// No element to pop (empty queue OR next elmt's get_up time is to big)
	if(!queue->head || (queue->head && queue->head->get_up > get_up))
		return NULL;

	// Else, there is an elemt to pop :
	// Update the new head of the queue;
	if(!empty_p(queue))
		queue->head = queue->head->next;

	// The process is removed from the queue, so it has no next proc any more
	if(tmp != NULL)
		tmp->next = NULL;

	return tmp;
}


/**
 * @param queue is the queue of process.
 * @return true if the queue is empty, false else.
 */
bool empty_p(struct proc_priority_queue* queue)
{
	return queue->head == NULL;
}
