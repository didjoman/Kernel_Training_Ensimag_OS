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
	struct Process* next;
};

struct proc_queue{
        struct Process* head;
        struct Process* tail;
};


void idle(void);
void proc1(void);
int32_t cree_processus(void (*code)(void), char *nom);


#endif
