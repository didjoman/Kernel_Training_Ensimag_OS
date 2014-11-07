#include "scheduler_coop.h"


struct Process proc_table[PROC_NB];
int32_t actif;

static int32_t mon_pid(void);
static char* mon_nom(void);
static void ordonnance(void);

extern void ctx_sw(int32_t* a, int32_t* b);

/*
void idle(void)
{
	for(int i = 0; i < 3; ++i){
		printf("[idle] je tente de passer la main a proc1...\n");
		ctx_sw(proc_table[0].save_zone, proc_table[1].save_zone);
	}
}

void proc1(void)
{
	for(int i = 0; i < 3; ++i){
		printf("[proc1] idle m'a donne la main\n");
		printf("[proc1] j'arrete le systeme\n");
		ctx_sw(proc_table[1].save_zone, proc_table[0].save_zone);
	}
	hlt();
}
*/
/*
void idle(void)
{
	for (int i = 0; i < 3; i++) {
		printf("[idle] je tente de passer la main a proc1...\n");
		ctx_sw(proc_table[0].save_zone, proc_table[1].save_zone);
		printf("[idle] proc1 m'a redonne la main\n");
	}
	printf("[idle] je bloque le systeme\n");
	hlt();
}

void proc1(void)
{

	for (int i = 0; i < 3; i++) {
		printf("[proc1] idle m'a donne la main\n");
		printf("[proc1] je tente de lui la redonner...\n");
		ctx_sw(proc_table[1].save_zone, proc_table[0].save_zone);
	}
}
//*/
void idle(void)
{
	for (;;) {
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		for (int32_t i = 0; i < 100000000; i++);
		ordonnance();
	}
}

void proc1(void) {
	for (;;) {
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		for (int32_t i = 0; i < 100000000; i++);
		ordonnance(); 
	}
}

void set_pid_of_ELU(int32_t pid)
{
	actif = pid;
}

struct Process* get_elu(void)
{
	for(int i = 0; i < PROC_NB; ++i)
		if(proc_table[i].s == ELU)
			return &proc_table[i];
		
	return (struct Process*)NULL;
}

static int32_t mon_pid(void)
{
	struct Process* elu = get_elu();
	return (elu != NULL) ? elu->pid : -1;
}

static char* mon_nom(void)
{
	struct Process* elu = get_elu();
	return (elu != NULL) ? elu->name : "merde";
}

struct Process* get_proc_table(void)
{
	return proc_table;
}

void ordonnance(void)
{

	struct Process* old = proc_table + actif;
	actif = ((proc_table + actif)->pid + 1) % PROC_NB;
	struct Process* new = proc_table + actif;
	old->s = ACTIVABLE;
	new->s = ELU;
	ctx_sw(old->save_zone, new->save_zone);
}

// actif : pid ou process directement.
