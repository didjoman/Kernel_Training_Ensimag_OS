#include "scheduler_coop.h"

// *** Static Function Definition : 
static int32_t mon_pid(void);
static char* mon_nom(void);
static void ordonnance(void);
extern void ctx_sw(int32_t* a, int32_t* b);

// *** Global Variables :
struct Process proc_table[PROC_NB];
struct Process* elected_proc;

/// *** Function definition :

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
 * Sets the pid of the process that will be executed in first.
 * @param pid is the pid of the process to execute in first.
*/
void set_elected_proc(struct Process* proc)
{
	elected_proc = proc;
}

/**
 * @return the process that is currently elected 
 *          or NULL if there is no elected process.
*/
struct Process* get_elected_proc(void)
{
	return elected_proc;
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
 * @ returns the processus table 
*/
struct Process* get_proc_table(void)
{
	return proc_table;
}

/**
 * Uses the Round-Robin Scheduling to decide wich process will have the CPU next
*/
void ordonnance(void)
{
	// We set the statue of the current elected proc to ACTIVABLE
	struct Process* former_elected = proc_table + elected_proc->pid;
	former_elected->state = ACTIVABLE;
	// Get the next process in the proc table and set its state to ELU
	elected_proc = &proc_table[((proc_table + elected_proc->pid)->pid + 1)
				  % PROC_NB];
	elected_proc->state = ELU;
	// Switch the current elected process and the next one :
	ctx_sw(former_elected->save_zone, 
	       proc_table[elected_proc->pid].save_zone);
}

