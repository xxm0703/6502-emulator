#include <stdint.h>

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

void setup_machine(machine_t *);
int run(machine_t *);

uint8_t inline read(uint16_t pc) return mach.ins[pc];
uint16_t inline rel(uint8_t inp) return inp + read(inp + 1);
uint16_t inline norm(uint8_t *src, uint16_t inx) return src[inx] | src[inx + 1] << 8;
uint8_t inline get(uint16_t abs) return mach.mem[abs];

uint16_t inline ind(uint8_t add) return norm(mach.mem, norm(mach.ins, add));
uint16_t inline indx(uint8_t zpg) return norm(mach.mem, read(zpg) + mach.cpu->X);
uint16_t inline indy(uint8_t zpg) return norm(mach.mem, read(zpg)) + mach.cpu->Y;

uint16_t inline zpg(uint8_t zpg) return read(zpg);
uint16_t inline zpgx(uint8_t add) return zpg(add) + mach.cpu->X;
uint16_t inline zpgy(uint8_t add) return zpg(add) + mach.cpu->Y;

uint16_t inline absm(uint8_t abs) return norm(mach.ins, abs);
uint16_t inline absx(uint8_t abs) return absm(abs) + mach.cpu->X;
uint16_t inline absy(uint8_t abs) return absm(abs) + mach.cpu->Y;




#endif
