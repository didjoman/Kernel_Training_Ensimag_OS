#include "scheduler_coop.h"
#include <malloc.c.h>

// *** Static Function Definition : 
static int32_t mon_pid(void);
static char* mon_nom(void);
static void ordonnance(void);
extern void ctx_sw(int32_t* a, int32_t* b);

// *** Global Variables :
struct Process* proc_table[PROC_NB];
int32_t last_proc_id = -1;
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
struct Process** get_proc_table(void)
{
	return proc_table;
}

/**
 * Uses the Round-Robin Scheduling to decide wich process will have the CPU next
*/
void ordonnance(void)
{

	// We set the statue of the current elected proc to ACTIVABLE
	struct Process* former_elected = elected_proc;
	// If there is no process, there is nothing to schedule
	if(former_elected == NULL)
		return;
	// Else the current process goes from ELU to ACTIVABLE state
	set_state_of_process(former_elected, ACTIVABLE);
	// We get the next process in the proc table and set its state to ELU
	elected_proc = proc_table[(former_elected->pid + 1) % PROC_NB];
	if(elected_proc == NULL)
		elected_proc = proc_table[0];
	set_state_of_process(elected_proc, ELU);
	
	// Switch the current elected process and the next one :
	ctx_sw(former_elected->save_zone, 
	       proc_table[elected_proc->pid]->save_zone);
}

int32_t cree_processus(void (*code)(void), char *nom)
{
	// We have created to much processes, we can not creat one more
	if(last_proc_id + 1 > PID_MAX)
		return -1;

	// If possible we create the process : 
	++last_proc_id;
        proc_table[last_proc_id] = (struct Process*) malloc(sizeof(struct Process));

        proc_table[last_proc_id]->pid = last_proc_id;
        strncpy(proc_table[last_proc_id]->name, nom, NAME_SIZE);
	proc_table[last_proc_id]->state = ACTIVABLE;

        // At the first execution the context of proc1 has never been saved.
        // |- We need to store a pointer on the stack in the register save zone
        proc_table[last_proc_id]->save_zone[ESP_REG_ID] =
                (int32_t)(proc_table[last_proc_id]->stack+(STACK_SIZE - 1));
        // |- The stack normally stocks the adress of the function that were
        // |  running before the interruption of the process :
        proc_table[last_proc_id]->stack[STACK_SIZE - 1] = (int32_t)code;
        // |- Rq : the stack is fulfilled from the higher index

	return last_proc_id;
}

void set_state_of_process(struct Process* process, enum state s)
{
	process->state = s;
	if(s == ELU)
		set_elected_proc(process);

}

void set_state_by_pid(int32_t pid, enum state s)
{
	struct Process* process = get_process(pid);

	if(process != NULL)
		set_state_of_process(process, s);	
}

struct Process* get_process(int32_t pid)
{
	return proc_table[pid];
}
