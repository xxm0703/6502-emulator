#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef x6502_H
#define x6502_H

#define C 1
#define Z 2
#define I 4
#define D 8
#define B 16
#define V 64
#define N 128

#define cary(x) if(x){cpu->flags |= C;}else{cpu->flags &= ~C;}
#define zero(x) if(x){cpu->flags &= ~Z;}else{cpu->flags |= Z;}
#define nega(x) if(x >> 7){cpu->flags |= N;}else{cpu->flags &= ~N;}
#define ovrf(x) if(x){cpu->flags |= V;}else{cpu->flags &= ~V;}

typedef struct {
        int8_t flags, acc, X, Y;
        uint8_t SP;
        uint16_t PC;
} cpu_t;

typedef struct {
        cpu_t *cpu;
        int8_t *mem, *ins;
} machine_t;

machine_t mach;

void setup_machine(machine_t *);
int run(machine_t *);

#endif
