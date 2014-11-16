#ifndef SCHEDULER_COOP_H
#define SCHEDULER_COOP_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cpu.h>
#include "process_queue.h"
#include "process.h"

#define PID_MAX 1000

void idle(void);
void proc1(void);
int32_t cree_processus(void (*code)(void), char *nom);
void init(void);

#endif
