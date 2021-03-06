#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cpu.h>
#include "process_queue.h"
#include "process_priority_queue.h"
#include "process.h"
#include "uptime.h"

#define PID_MAX 10

void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);
void proc4(void);
int32_t cree_processus(void (*code)(void), char *nom);
void ordonnance(void);
void init(void);
void dors(time_t nbr_secs);
void test(void);
#endif
