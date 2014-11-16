#include "scheduler_coop.h"

// *** Static Function Definition : 
static int32_t mon_pid(void);
static char* mon_nom(void);
static void ordonnance(void);
extern void ctx_sw(int32_t* a, int32_t* b);
static void load_elected();
static void unload_elected();

// *** Global Variables :
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
	// Switch the current elected process and the next one :
	ctx_sw(former_elected->save_zone, elected_proc->save_zone);
}

// TODO : Commenter
int32_t cree_processus(void (*code)(void), char *nom)
{
	// We have created to much processes, we can not creat one more
	if(last_proc_id + 1 > PID_MAX)
		return -1;

	// If possible we create the process : 
        struct Process* proc = new_process(++last_proc_id, ACTIVABLE, code, nom);
	
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
