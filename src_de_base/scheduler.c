#include "scheduler.h"

// *** Static Function Definition : 
static time_t nbr_secondes(void);
static int32_t mon_pid(void);
static char* mon_nom(void);
static void load_elected();
static void unload_elected();
static void wake_up(void);
static void fin_processus(void);
static void free_dyings(void);

// *** External Functions :
extern void ctx_sw(int32_t* a, int32_t* b);

// *** Global Variables :
int32_t last_proc_id = -1;
struct Process* elected_proc;

// *** Data Structures :
struct proc_priority_queue sleepings;
struct proc_queue activables;
struct proc_queue dyings;

// *** Function definition :

/**
 * Init creates the idle process (running when there is no other program to run)
 * and some other processes (for the demonstration). 
*/
void init(void)
{
	// We create the idle :
	(void) cree_processus(idle, "idle");
	load_elected();
	// We add other processes for the tests :
        (void)cree_processus(proc1, "proc1");
        (void)cree_processus(proc2, "proc2");
        (void)cree_processus(proc3, "proc3");
        (void)cree_processus(proc4, "proc4");
}

/**
 * Function describing the execution of process Idle
*/
void idle()
{
	for (;;) {
		sti(); // Active les interruptions
		hlt(); // bloque le processus jusqu'a la réception de la prochaine IT
		cli(); // désactive les interruptions
	}
}

/**
 * Function describing the execution of process proc1
 * Proc1 enables us to test the behavior of a process finishing.
 * (after two turns, proc1 goes in MOURANT state and is freed.)
*/
void proc1(void)
{
	static int nb_proc1_created = 0;
	for (int32_t i = 0; i < 2; i++) {
		printf("[temps = %u] processus %s pid = %i (dort 2secs)\n", nbr_secondes(), mon_nom(), mon_pid());
		// Test of dynamic creation of process :
		char name[5] = "proc1";
		char new_name[20];
		snprintf(new_name, 20, "%s_%d", name, nb_proc1_created++);
		(void)cree_processus(proc1, new_name);
		dors(2);
		// Awaited behavior : Each proc1 creates an other one (recursively) 
		// AND : there will be only 6 process created because
		//       PID_MAX = 10 and we still have 4 process created.
	}
}

/**
 * Function describing the execution of process proc2
*/
void proc2(void)
{
	for (;;) {
		printf("[temps = %u] processus %s pid = %i (dort 3secs)\n", nbr_secondes(), mon_nom(), mon_pid());
		dors(3);
	}
}

/**
 * Function describing the execution of process proc3
*/
void proc3(void)
{
	for (;;) {
		printf("[temps = %u] processus %s pid = %i (dort 5secs)\n", nbr_secondes(), mon_nom(), mon_pid());
		dors(5);
	}
}

/**
 * Function describing the execution of process proc3
*/
void proc4(void)
{
	for (;;) {
		printf("[temps = %u] processus %s pid = %i (dort 6secs)\n", nbr_secondes(), mon_nom(), mon_pid());
		dors(6);
	}
}


static time_t nbr_secondes(void)
{
	return get_uptime();
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
 * Basically, it wakes up the sleeping processes, unload the elected process, 
 * loads an other one from the activable process queue, swaps the contexts, 
 * and free the dying processes.
*/
void ordonnance(void)
{
	// We start by waking up the sleeping processes (add them to activables)
	wake_up();
	// We set the statue of the current elected proc to ACTIVABLE
	struct Process* former_elected = elected_proc;
	// Else the current elected returns to the list of activable processes
	unload_elected();
	// We get the next activable process & set its state to ELU
	load_elected();
	// Switch the current elected process and the next one :
	ctx_sw(former_elected->save_zone, elected_proc->save_zone);

	// We free all the dying processes.
	free_dyings();
}


/**
 * If possible (PID_MAX not reached), this function allocates a new process 
 * and adds it to the queue of acivable processes.
 * @param code is the code to be executed by the process
 * @param nom is the name given to the process
 * @return the pid of the process created, or -1 if no porcess has been created
 */
int32_t cree_processus(void (*code)(void), char *nom)
{
	// We have created to much processes, we can not creat one more
	if(last_proc_id + 1 > PID_MAX)
		return -1;

	// If possible we create the process : 
        struct Process* proc = new_process(++last_proc_id, ACTIVABLE, code, 
					   fin_processus, nom);
	
	// We add the new process to the list of activables porcesses
	push(&activables, proc);
	
	return last_proc_id;
}

/**
 * Loads a new elected process from the queue of activable.
*/
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

/**
 * Unloads the current elected process to the appropriate queue depending on
 * its state.
 * By default, the elected process returns to the queue of activables.
 * It can also be queued to :
 * - the sleeping queue if he is sleeping (after a call to dors()).
 * - the dyings queue if it is dying (before to be free).
*/
static void unload_elected()
{
	if(elected_proc == NULL) 
		return;

	if(elected_proc->state == ELU){
		// If the process is still in the state of elected at 
		// the end of its "turn", it goes in the activable queue
		elected_proc->state = ACTIVABLE;
		push(&activables, elected_proc);
	} else if(elected_proc->state == ENDORMI)
		// If the state has been changed to sleeping during the exe
		//Then we insert it in the list of sleeping processes.
		push_p(&sleepings, elected_proc);
	else if(elected_proc->state == MOURANT)
		// If the state is MOURANT, we add the proc' to the dying queue
		// This process will next be freed in ordonnance().
		push(&dyings, elected_proc);		
}

/**
 * This function changes the state of the elected process to ENDORMI
 * and calls ordonnance() so that the process could be queued to the
 * sleeping queue.
 *
 * @param nbr_secs is the time in seconds the process should sleep.
*/
void dors(time_t nbr_secs)
{
	// We get the current elected process and affect it a get_up time
	elected_proc->get_up = get_uptime() + (nbr_secs * 1000);
	// We set the new state of the process
	elected_proc->state = ENDORMI;
	// Ordonnance() will unload the process in sleeping queue and load a new one 
	ordonnance();
}

/**
 * This function awakes all the sleeping processes for wich the 
 * get_up time is less or equal to the current uptime.
 */
void wake_up(void)
{
	time_t current_time = get_uptime();
	struct Process* p;
	while((p = pop_p(&sleepings, current_time)) != NULL){
		p->state = ACTIVABLE;
		push(&activables, p);
	}
}

/**
 * This function changes the state of the elected process to MOURANT
 * and calls ordonnance() so that the process could be queued to the
 * dying queue before to be freed.
 */
void fin_processus(void){
	printf(" kill called\n");
	// We set the state to Mourant
	elected_proc->state = MOURANT;
	// Then ordonnance() will add the process to dying queue and free it
	ordonnance();
}

/**
 * This function frees all the processes contained in the dying queue.
 */
void free_dyings(void){
	struct Process* p;
	while((p = pop(&dyings)) != NULL)
		destroy_process(p);
}

