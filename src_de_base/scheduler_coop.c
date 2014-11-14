#include "scheduler_coop.h"
#include <malloc.c.h>

// *** Static Function Definition : 
static int32_t mon_pid(void);
static char* mon_nom(void);
static void ordonnance(void);
extern void ctx_sw(int32_t* a, int32_t* b);
static void load_elected();
static void unload_elected();

static void push(struct proc_queue* queue, struct Process* p);
static struct Process* pop(struct proc_queue* queue);
static bool empty(struct proc_queue* queue);


// *** Global Variables :
//struct Process* proc_table[PROC_NB];
struct proc_queue activables;
int32_t last_proc_id = -1;
struct Process* elected_proc;

/// *** Function definition :

// TODO : l'idle devrait etre lancée uniquement lorsqu'il n'y a pas d'autres processus activables ..
void init(void)
{
	(void) cree_processus(idle, "idle");
	load_elected();
}

/**
 * Function describing the execution of process Idle
*/
void idle(void)
{
	for (;;) {
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		for (int32_t i = 0; i < 100000000; i++);
		ordonnance();
	}
}

/**
 * Function describing the execution of process proc1
*/
void proc1(void) {
	for (;;) {
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		for (int32_t i = 0; i < 100000000; i++);
		ordonnance(); 
	}
}

/**
 * @return the pid of the process that is currently elected 
 *          or -1 if there is no elected process.
*/
static int32_t mon_pid(void)
{
	return (elected_proc != NULL) ? elected_proc->pid : -1;
}

/**
 * @return the name of the process that is currently elected 
 *          or an empty string if there is no elected process.
*/
static char* mon_nom(void)
{
	return (elected_proc != NULL) ? elected_proc->name : "";
}

/**
 * Uses the Round-Robin Scheduling to decide wich process will have the CPU next
*/
void ordonnance(void)
{

	// We set the statue of the current elected proc to ACTIVABLE
	struct Process* former_elected = elected_proc;
	// Else the current elected returns to the list of activable processes
	unload_elected();
	// We get the next activable process & set its state to ELU
	load_elected();

	// If there is no activable process, there is nothing to schedule
	if(elected_proc == NULL)
		return;

	// Switch the current elected process and the next one :
	if(former_elected == NULL) // case in witch no processes where running
		former_elected = elected_proc;

	ctx_sw(former_elected->save_zone, elected_proc->save_zone);
}

// TODO : Commenter
int32_t cree_processus(void (*code)(void), char *nom)
{
	// We have created to much processes, we can not creat one more
	if(last_proc_id + 1 > PID_MAX)
		return -1;

	// If possible we create the process : 
	++last_proc_id;

        struct Process* proc = (struct Process*) malloc(sizeof(struct Process));
        proc->pid = last_proc_id;
        strncpy(proc->name, nom, NAME_SIZE);
	proc->state = ACTIVABLE;

        // At the first execution the context of proc1 has never been saved.
        // |- We need to store a pointer on the stack in the register save zone
        proc->save_zone[ESP_REG_ID] =
                (int32_t)(proc->stack+(STACK_SIZE - 1));
        // |- The stack normally stocks the adress of the function that were
        // |  running before the interruption of the process :
        proc->stack[STACK_SIZE - 1] = (int32_t)code;
        // |- Rq : the stack is fulfilled from the higher index
	
	// We add the new process to the list of activables porcesses
	push(&activables, proc);
	
	return last_proc_id;
}

// TODO faire fonction init qui met un processus idle en proc elu ! 
/*
le processus idle (qui a par convention le pid 0) est le processus par défaut : en effet, dans un système, il doit toujours y avoir au moins un processus actif, car le système ne doit jamais s'arrêter ; 
*/
// TODO : Commenter
static void load_elected()
{
	// We load the next activable process
	struct Process* proc = pop(&activables);

	if(proc == NULL)
		return;

	// If there is one, it becomes the new elected process
	elected_proc = proc;
	elected_proc->state = ELU;
}

// TODO : Commenter
static void unload_elected()
{
	if(elected_proc == NULL) 
		return;

	elected_proc->state = ACTIVABLE;
	push(&activables, elected_proc);
}

/******************************************************************************/
/*************************** DATA STRUCTURE : QUEUE ***************************/
/******************************************************************************/

// TODO : put it in an other file;

/**
 * Add Process in tail
 * @param queue is the queue we want to add the process to.
 * @param p is a pointer to the process to insert on the queue 
*/
static void push(struct proc_queue* queue, struct Process* p)
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
static struct Process* pop(struct proc_queue* queue)
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
static bool empty(struct proc_queue* queue)
{
	return queue->head == NULL;
}
