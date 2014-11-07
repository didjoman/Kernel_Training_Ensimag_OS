#ifndef SCHEDULER_COOP_H
#define SCHEDULER_COOP_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cpu.h>

#define PROC_NB 2
#define STACK_SIZE 512
#define NAME_SIZE 20
#define SAVE_ZONE_SIZE 5

enum state{ELU, ACTIVABLE};

struct Process {
	int pid;
	char name[NAME_SIZE];
	enum state state;
	int32_t save_zone[SAVE_ZONE_SIZE];
	int32_t stack[STACK_SIZE];
};

void idle(void);
void proc1(void);
struct Process* get_elected_proc(void);
void set_elected_proc(struct Process* proc);
struct Process* get_proc_table(void);
#endif
