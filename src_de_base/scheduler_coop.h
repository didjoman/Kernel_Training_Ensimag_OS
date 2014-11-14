#ifndef SCHEDULER_COOP_H
#define SCHEDULER_COOP_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cpu.h>


#define PROC_NB 1000
#define STACK_SIZE 512
#define NAME_SIZE 20
#define SAVE_ZONE_SIZE 5
#define PID_MAX 1000
#define ESP_REG_ID 1

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
struct Process** get_proc_table(void);
int32_t cree_processus(void (*code)(void), char *nom);
struct Process* get_process(int32_t pid);
void set_state_by_pid(int32_t pid, enum state s);
void set_state_of_process(struct Process* process, enum state s);


#endif
