#include "process.h"
#include <malloc.c.h>

//extern void fin_processus(void);

struct Process* new_process(int pid, enum state state, void (*code)(void),
			    void (*fin)(void), char *name)
{
	// Allocation of a new pointer pointing to the Process data struct.
        struct Process* proc = (struct Process*) malloc(sizeof(struct Process));
	if(!proc){
		fprintf(stderr, "Allocation of a new process failed.");
		//exit(EXIT_FAILURE); // NOT IMPLEMENTED IN STDLIB.H ...
	}

	// We fill in the basic informations.
        proc->pid = pid;
        strncpy(proc->name, name, NAME_SIZE);
        proc->state = state;

        // At the first execution, the context of proc1 has never been saved.
        // |- We need to store 2 pointers to the stack in the register save zone
        proc->save_zone[EBP_REG_ID] =
                (int32_t)(proc->stack+(STACK_SIZE - 1));
        proc->save_zone[ESP_REG_ID] =
                (int32_t)(proc->stack+(STACK_SIZE - 2));
	// |- We add a function used to end the process execution
	proc->stack[STACK_SIZE - 1] = (int32_t)fin;
        // |-  + The stack normally stocks the adress of the function that were
        // |  running before the interruption of the process :
        proc->stack[STACK_SIZE - 2] = (int32_t)code;
        // |- Rq : the stack is fulfilled from the higher index

	return proc;
}

void destroy_process(struct Process* p)
{
	free(p);
}
