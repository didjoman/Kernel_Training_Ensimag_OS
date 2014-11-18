#include <string.h>
#include <stdio.h>
#include <stdbool.h>                                           
#include <inttypes.h>
#include <cpu.h>
#include "put_bytes.h"
#include "segment.h"
#include "scheduler.h"

#ifndef UPTIME_H
#define UPTIME_H

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

typedef uint32_t time_t;

void init_clock();
void print_uptime(char * time);
void tic_PIT(void);
void init_traitant_IT(int32_t num_IT, void (*traitant) (void));
void masque_IRQ(uint32_t num_IRQ, bool masque);
time_t get_uptime();


#endif
