#ifndef PROCESS_H
#define PROCESS_H


#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uptime.h"

#define STACK_SIZE 512
#define NAME_SIZE 20
#define SAVE_ZONE_SIZE 5
// Cf traitant.S to know the reg numbers : 
#define ESP_REG_ID 1 // id of the register %esp in the save_zone
#define EBP_REG_ID 2 // id of the register %ebp in the save_zone

enum state{ELU, ACTIVABLE, ENDORMI, MOURANT};

struct process_t {
        int pid;
        char name[NAME_SIZE];
        enum state state;
        int32_t save_zone[SAVE_ZONE_SIZE];
        int32_t stack[STACK_SIZE];
	time_t get_up;
        struct process_t* next;
};

struct process_t* new_process(int pid, enum state state, void (*code)(void), 
			    void (*fin)(void), char *name);
void destroy_process(struct process_t* p);

#endif
